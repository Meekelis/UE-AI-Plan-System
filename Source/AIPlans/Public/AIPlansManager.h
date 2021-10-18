// Copyright: Kazimieras Mikelis

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIPlansManager.generated.h"

class UAIPlan;
/**
 * Manager for managing AI plan lifetime.
 */
UCLASS(Config=Game, meta=(DisplayName="AI Plans Manager"))
class AIPLANS_API UAIPlansManager : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:

	/**
	 * Lifetime for all AI actions after their execution has finished.
	 */
	UPROPERTY(BlueprintReadOnly, Config)
	float LifetimeAfterExecutionStopped = 30.f;

	/**
	 * See whether a controller is running an AI plan.
	 */
	UFUNCTION(BlueprintPure, Category = "AI Plans")
	static bool IsRunningPlan(const AAIController* Controller);

	/**
	 * Get the current AI plan of a controller.
	 */
	UFUNCTION(BlueprintPure, Category = "AI Plans", meta=(DisplayName = "Get AI Plan"))
	static UAIPlan * GetAIPlan(const AAIController* Controller);

	// Called to add an AI plan to a set of plans which will not be garbage collected.
	static void RegisterPlan(AAIController* Controller, UAIPlan* Plan);

	// Called to move an AI plan of a controller to a delete buffer, where it will stay for LifetimeAfterExecutionStopped before being garbage collected.
	static void DeregisterPlan(const AAIController* Controller);

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}

private:

	/**
	 * A map of all running plans.
	 */
	UPROPERTY()
	TMap<AAIController*, UAIPlan*> RunningPlans;

	/**
	 * A list of plans to be deleted.
	 */
	UPROPERTY()
	TMap<UAIPlan*, float> DeleteBuffer;

	// Gets running plans statically.
	static TMap<AAIController*, UAIPlan*>* StaticallyGetRunningPlans();

	// Gets the manager statically.
	static UAIPlansManager* Get();
	
};
