// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BHMemoryComponent.h"
#include "BHPlanner.h"
#include "UObject/Interface.h"
#include "BHGOAPAgent.generated.h"


UINTERFACE(MinimalAPI, BlueprintType)
class UBHGOAPAgent : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface have to be implemented by each actor class that want to use GOAP planner.
 */
class BRAINHACKER_API IBHGOAPAgent
{
	GENERATED_BODY()

public:

	/** Return agent's planner component. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UBHPlanner* GetGOAPPlanner();

	/** Return agent's memory component. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UBHMemoryComponent* GetGOAPMemoryComponent();
};
