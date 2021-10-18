// Copyright: Kazimieras Mikelis

#include "K2Node_RunAIAction.h"
#include "AIController.h"
#include "AIPlan.h"
#include "AIPlansStatics.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "K2Node_RunAIAction"

struct FK2Node_RunAIActionHelper
{
	static const FName ControllerPinName;
};

const FName FK2Node_RunAIActionHelper::ControllerPinName(TEXT("AIController"));

void UK2Node_RunAIAction::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	if(const auto Pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, AAIController::StaticClass(), FK2Node_RunAIActionHelper::ControllerPinName))
	{
		Pin->PinFriendlyName = LOCTEXT("RunAIPlanInstigatorPin", "AI Controller");
	}
}

FText UK2Node_RunAIAction::GetNodeTitle(ENodeTitleType::Type Title) const
{
	UEdGraphPin* ClassPin = GetClassPin();
	if (!ClassPin || !IsValid(ClassPin->DefaultObject) || ClassPin->LinkedTo.Num())
	{
		return LOCTEXT("RunAIPlanTitleUnknown", "Run AI Plan");
	}

	if (CachedNodeTitle.IsOutOfDate(this))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("Plan"), FText::FromString(ClassPin->DefaultObject->GetName()));
		CachedNodeTitle.SetCachedText(FText::Format(LOCTEXT("RunAIPlanTitleKnown", "Run {Plan}"), Args), this);
	}
	return CachedNodeTitle;
}

void UK2Node_RunAIAction::GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	if (UEdGraphPin* Pin = GetAIControllerPin())
	{
		K2Schema->ConstructBasicPinTooltip(*Pin, LOCTEXT("RunAIPlanAIControllerDescription", "The AI controller for which to run the plan."), Pin->PinToolTip);
	}

	return Super::GetPinHoverText(Pin, HoverTextOut);
}

void UK2Node_RunAIAction::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	static FName RunPlanFunctionName = GET_FUNCTION_NAME_CHECKED(UAIPlansStatics, RunAIPlan);
	static FName FunctionParam_PlanClass = TEXT("Plan");
	static FName FunctionParam_Instigator = TEXT("Instigator");

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* InstigatorPin = GetAIControllerPin();
	UEdGraphPin* PlanClassPin = GetClassPin();
	UEdGraphPin* ThenPin = GetThenPin();
	UEdGraphPin* ResultPin = GetResultPin();

	UClass* SpawnClass = PlanClassPin ? Cast<UClass>(PlanClassPin->DefaultObject) : nullptr;
	if (!PlanClassPin->LinkedTo.Num() && !SpawnClass)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("RunAIPlanExpandingWithoutClass", "Spawn node @@ must have a class specified.").ToString(), this);
		BreakAllNodeLinks();
		return;
	}

	if (!InstigatorPin || !InstigatorPin->HasAnyConnections())
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("RunAIPlanExpandingWithoutInstigator", "Spawn node @@ must have an AI controller linked.").ToString(), this);
		BreakAllNodeLinks();
		return;
	}

	// Here we adapt/bind our pins to the static function pins that we are calling.
	UK2Node_CallFunction* CallCreateNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallCreateNode->FunctionReference.SetExternalMember(RunPlanFunctionName, StaticClass());
	CallCreateNode->AllocateDefaultPins();

	//allocate nodes for created widget.
	UEdGraphPin* CallCreate_ExecPin = CallCreateNode->GetExecPin();
	UEdGraphPin* CallCreate_PlanClass = CallCreateNode->FindPinChecked(FunctionParam_PlanClass);
	UEdGraphPin* CallCreate_Instigator = CallCreateNode->FindPinChecked(FunctionParam_Instigator);
	UEdGraphPin* CallCreate_Result = CallCreateNode->GetReturnValuePin();

	if (PlanClassPin->LinkedTo.Num())
	{
		CompilerContext.MovePinLinksToIntermediate(*PlanClassPin, *CallCreate_PlanClass);
	}
	else
	{
		CallCreate_PlanClass->DefaultObject = SpawnClass;
	}

	CallCreate_Result->PinType = ResultPin->PinType;

	CompilerContext.MovePinLinksToIntermediate(*InstigatorPin, *CallCreate_Instigator);
	CompilerContext.MovePinLinksToIntermediate(*ResultPin, *CallCreate_Result);
	CompilerContext.MovePinLinksToIntermediate(*ExecPin, *CallCreate_ExecPin);

	// Now we make the assignment pins for all exposed class variables.
	UEdGraphPin* LastThen = FKismetCompilerUtilities::GenerateAssignmentNodes(CompilerContext, SourceGraph, CallCreateNode, this, CallCreate_Result, GetClassToSpawn());

	CompilerContext.MovePinLinksToIntermediate(*ThenPin, *LastThen);

	// Finally, break stuff that was connected.
	BreakAllNodeLinks();
}

FSlateIcon UK2Node_RunAIAction::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = FLinearColor(0.125f, 0.f, 0.95f);
	static FSlateIcon Icon("EditorStyle", "GraphEditor.SpawnActor_16x");
	return Icon;
}

bool UK2Node_RunAIAction::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(TargetGraph);
	return Super::IsCompatibleWithGraph(TargetGraph) && (!Blueprint || (FBlueprintEditorUtils::FindUserConstructionScript(Blueprint) != TargetGraph && Blueprint->GeneratedClass->GetDefaultObject()->ImplementsGetWorld()));
}

FText UK2Node_RunAIAction::GetTooltipText() const
{
	return LOCTEXT("RunAIPlanTooltip", "Attempts to run an AI plan.");
}

FText UK2Node_RunAIAction::GetMenuCategory() const
{
	return LOCTEXT("RunAIPlanMenuCategory", "AI Plans");
}

UClass* UK2Node_RunAIAction::GetClassPinBaseClass() const
{
	return UAIPlan::StaticClass();
}

bool UK2Node_RunAIAction::IsSpawnVarPin(UEdGraphPin* Pin) const
{
	const FName PinName = Pin->PinName;
	return Super::IsSpawnVarPin(Pin) && PinName != FK2Node_RunAIActionHelper::ControllerPinName;
}

UEdGraphPin* UK2Node_RunAIAction::GetAIControllerPin() const
{
	return FindPinChecked(FK2Node_RunAIActionHelper::ControllerPinName);
}

#undef LOCTEXT_NAMESPACE