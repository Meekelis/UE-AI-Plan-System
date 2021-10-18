// Copyright: Kazimieras Mikelis


#include "AIPlansStatics.h"

UAIPlan* UAIPlansStatics::RunAIPlan(const TSubclassOf<UAIPlan> Plan, AAIController* Instigator)
{
	if(!ensureAlwaysMsgf(IsValid(Instigator), TEXT("UAIPlansStatics::RunAIPlan(): instigator AI controller is not valid.")))
	{
		return nullptr;
	}

	if (!ensureAlwaysMsgf(IsValid(Plan), TEXT("UAIPlansStatics::RunAIPlan(): plan class is not valid.")))
	{
		return nullptr;
	}

	UAIPlan* NewPlan = NewObject<UAIPlan>(GetTransientPackage(), Plan);
	if (!ensureAlwaysMsgf(IsValid(NewPlan), TEXT("UAIPlansStatics::RunAIPlan(): could not spawn a plan.")))
	{
		return nullptr;
	}

	// This will announce the plan to the AIPlansManager, where it will be safe from garbage collection.
	NewPlan->Initialize(Instigator);
	return NewPlan;
}