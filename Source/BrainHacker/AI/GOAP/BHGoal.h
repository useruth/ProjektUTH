// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BHGOAPTypes.h"
#include "BHPlanner.h"
#include "BHGoal.generated.h"

class IBHGOAPAgent;

/**
 * Goal for planner. Defines what AI want to achieve and how important it is.
 */
UCLASS(Blueprintable, BlueprintType)
class BRAINHACKER_API UBHGoal : public UObject
{
	GENERATED_BODY()

public:

	friend class UBHPlanner;

	/** Return true if goal is valid and can be considered in planning. */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsGoalValid();
	
	/** Return value in range <0,1>. 0 - goal isn't important. 1 - goal is very important. */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	float GetGoalScore();

	/** Return world state that satisfy goal. */
	UFUNCTION(BlueprintCallable)
	FBHWorldStateData GetDesiredWorldState();

protected:
	
	/**
	 * Update current desired world state.
	 * @warning have to be implemented in all classes
	 * @warning have to update value of DesiredWorldState
	 */
	UFUNCTION(BlueprintNativeEvent)
	void UpdateDesiredWorldState();

	/** Reference to actor for which this goal is generated (planner owner actor). */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TScriptInterface<IBHGOAPAgent> AgentActor;

	/** Current desired world state. Is updated on each GetDesiredWorldState use. */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FBHWorldStateData DesiredWorldState;
	
};