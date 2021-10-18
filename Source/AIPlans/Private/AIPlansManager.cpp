// Copyright: Kazimieras Mikelis


#include "AIPlansManager.h"

#include "AIPlan.h"

bool UAIPlansManager::IsRunningPlan(const AAIController* Controller)
{
	if(const auto Plans = StaticallyGetRunningPlans())
	{
		return Plans->Contains(Controller);
	}
	return false;
}

UAIPlan* UAIPlansManager::GetAIPlan(const AAIController* Controller)
{
	if (const auto Plans = StaticallyGetRunningPlans())
	{
		if(Plans->Contains(Controller))
		{
			return (*Plans)[Controller];
		}
	}
	return nullptr;
}

void UAIPlansManager::RegisterPlan(AAIController* Controller, UAIPlan* Plan)
{
	if (const auto Manager = Get())
	{
		if(Manager->DeleteBuffer.Contains(Plan))
		{
			Manager->DeleteBuffer.Remove(Plan);
		}

		Manager->RunningPlans.Add(Controller, Plan);
	}
}

void UAIPlansManager::DeregisterPlan(const AAIController* Controller)
{
	if (const auto Manager = Get())
	{
		if(Manager->RunningPlans.Contains(Controller))
		{
			Manager->DeleteBuffer.Add(Manager->RunningPlans[Controller], Manager->LifetimeAfterExecutionStopped);
		}

		Manager->RunningPlans.Remove(Controller);
	}
}

void UAIPlansManager::Tick(const float DeltaTime)
{
	TArray<UAIPlan*> PendingKill;

	for(auto & Plan : DeleteBuffer)
	{
		if((Plan.Value -= DeltaTime) <= 0.f)
		{
			PendingKill.AddUnique(Plan.Key);
		}
	}

	for(const auto Plan : PendingKill)
	{
		DeleteBuffer.Remove(Plan);
		Plan->ConditionalBeginDestroy();
	}
}

TMap<AAIController*, UAIPlan*>* UAIPlansManager::StaticallyGetRunningPlans()
{
	if(const auto Manager = Get())
	{
		return &Manager->RunningPlans;
	}

	return nullptr;
}

UAIPlansManager* UAIPlansManager::Get()
{
	const UWorld* GameWorld = nullptr;
	TIndirectArray<FWorldContext> Worlds = GEngine->GetWorldContexts();
	for (auto World : Worlds)
	{
		if (World.WorldType == EWorldType::Game || World.WorldType == EWorldType::PIE)
		{
			GameWorld = World.World();
			break;
		}
	}

	if (!ensureAlwaysMsgf(IsValid(GameWorld), TEXT("UAIPlansManager::StaticallyGetRunningPlans(): game world not valid.")))
	{
		return nullptr;
	}

	UAIPlansManager* Manager = GameWorld->GetSubsystem<UAIPlansManager>();
	if (!ensureAlwaysMsgf(IsValid(Manager), TEXT("UAIPlansManager::StaticallyGetRunningPlans(): manager not valid.")))
	{
		return nullptr;
	}

	return Manager;
}
