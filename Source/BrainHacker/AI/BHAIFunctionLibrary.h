// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAP/BHGOAPAgent.h"
#include "GOAP/BHGOAPTypes.h"
#include "GOAP/BHPlanner.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BHAIFunctionLibrary.generated.h"

/**
 * Utilities for AI.
 */
UCLASS()
class BRAINHACKER_API UBHAIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Get UBHPlanner from agent in BP. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UBHPlanner* GetGOAPPlanner(const TScriptInterface<IBHGOAPAgent>& ForAgent);

	/** Get UBHMemoryComponent from agent in BP. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UBHMemoryComponent* GetGOAPMemoryComponent(const TScriptInterface<IBHGOAPAgent>& ForAgent);
	
};
