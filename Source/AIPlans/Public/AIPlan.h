// Copyright: Kazimieras Mikelis

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UObject/NoExportTypes.h"
#include "AIPlan.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AIPLANS_API UAIPlan : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlanFinished, AAIController*, Controller, FName, FinishMessage);

	/**
	 * Delegate broadcast on when the plan has finished.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnPlanFinished OnPlanFinished;

	/**
	 * Execute this AI plan.
	 */
	UFUNCTION(BlueprintCallable)
	void Initialize(AAIController* Instigator);

	/**
	 * Abort this AI plan.
	 */
	UFUNCTION(BlueprintCallable)
	void Abort();

	virtual UWorld* GetWorld() const override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}

protected:

	/**
	 * Gracefully finish this AI plan.
	 * @param FinishMessage		The message with which this plan has ended.
	 */
	UFUNCTION(BlueprintCallable)
	void Finish(FName FinishMessage);

	/**
	 * Called when the plan should start executing.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnExecute(AAIController * Controller, APawn * ControlledPawn);

	/**
	 * Called when the plan should start executing.
	 */
	virtual void OnExecute_Implementation(AAIController* Controller, APawn* ControlledPawn) {}

	/**
	 * Called on tick.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnUpdate(AAIController* Controller, APawn* ControlledPawn);

	/**
	 * Called on tick.
	 */
	virtual void OnUpdate_Implementation(AAIController* Controller, APawn* ControlledPawn) {}

	/**
	 * Called when the plan should finish executing
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnStopExecute(AAIController* Controller, APawn* ControlledPawn, bool bAborted = false);

	/**
	 * Called when the plan should finish executing
	 */
	virtual void OnStopExecute_Implementation(AAIController* Controller, APawn* ControlledPawn, bool bAborted = false) {}

private:

	/**
	 * The controller that owns this plan.
	 */
	UPROPERTY()
	AAIController* Owner = nullptr;

	/**
	 * Whether this plan is currently executing. 
	 */
	UPROPERTY()
	bool bExecuting = false;

	/**
	 * Get the pawn controlled by the owner.
	 */
	UFUNCTION()
	APawn* GetControlledPawn();

	bool bQueuedExecution = false;

	
};
