// Copyright: Kazimieras Mikelis

#pragma once

#include "CoreMinimal.h"
#include "AIPlan.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AIPlansStatics.generated.h"

/**
 * Helper statics for running AI plans.
 */
UCLASS()
class AIPLANS_API UAIPlansStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * A function to start an AI plan. Called by a custom K2 node.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="AI Plans", meta = (BlueprintInternalUseOnly = "true"))
	static UAIPlan * RunAIPlan(TSubclassOf<UAIPlan> Plan, AAIController * Instigator);
};
