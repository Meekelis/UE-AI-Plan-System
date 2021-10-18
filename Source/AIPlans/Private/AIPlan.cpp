// Copyright: Kazimieras Mikelis


#include "AIPlan.h"

#include "AIPlansManager.h"

void UAIPlan::Initialize(AAIController* Instigator)
{
	if (!ensureAlwaysMsgf(!bExecuting, TEXT("UAIPlan::Execute(): already executing this plan.")))
	{
		return;
	}

	Owner = Instigator;
	bExecuting = true;
	bQueuedExecution = true;
	UAIPlansManager::RegisterPlan(Instigator, this);
}

void UAIPlan::Abort()
{
	if (!ensureAlwaysMsgf(bExecuting, TEXT("UAIPlan::Abort(): this plan is not being executed now.")))
	{
		return;
	}

	bExecuting = false;
	OnStopExecute(Owner, GetControlledPawn(), true);
	UAIPlansManager::DeregisterPlan(Owner);
	OnPlanFinished.Broadcast(Owner, TEXT("Special.Abort"));
}

void UAIPlan::Tick(float DeltaTime)
{
	if (bQueuedExecution)
	{
		bQueuedExecution = false;
		OnExecute(Owner, GetControlledPawn());
		return;
	}

	if(bExecuting) 
	{
		OnUpdate(Owner, GetControlledPawn());
	}
}

UWorld* UAIPlan::GetWorld() const
{
	UWorld* GameWorld = nullptr;
	TIndirectArray<FWorldContext> Worlds = GEngine->GetWorldContexts();
	for (auto World : Worlds)
	{
		if (World.WorldType == EWorldType::Game || World.WorldType == EWorldType::PIE)
		{
			GameWorld = World.World();
			break;
		}
	}

	return GameWorld;
}

void UAIPlan::Finish(const FName FinishMessage)
{
	if (!ensureAlwaysMsgf(bExecuting, TEXT("UAIPlan::Finish(): this plan is not being executed now.")))
	{
		return;
	}

	bExecuting = false;
	OnStopExecute(Owner, GetControlledPawn(), false);
	UAIPlansManager::DeregisterPlan(Owner);
	OnPlanFinished.Broadcast(Owner, FinishMessage);
}

APawn* UAIPlan::GetControlledPawn()
{
	if(!ensureMsgf(IsValid(Owner), TEXT("UAIPlan::GetControlledPawn(): no valid owner.")))
	{
		return nullptr;
	}

	return Owner->GetPawn();
}
