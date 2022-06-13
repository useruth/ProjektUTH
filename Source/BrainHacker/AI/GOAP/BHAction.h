// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BHWorldStateAtom.h"
#include "BrainHacker/GAS/BHGameplayAbility.h"
#include "BHAction.generated.h"

class IBHGOAPAgent;

/**
 * Ability that can be used by NPC with GOAP planner
 */
UCLASS()
class BRAINHACKER_API UBHAction : public UBHGameplayAbility
{
	GENERATED_BODY()

public:

	/** Return true if this ability change world state to desired value. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanChangeWorldState(FBHWorldStateData DesiredWorldState, AActor* AgentActor);

	/** Return preconditions that need to be satisfied to run ability. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FBHWorldStateData> GetWorldStatePreconditions(FBHWorldStateData ForDesiredWorldState, AActor* AgentActor);

	/** Set desired world state value which this ability will try to achieve after activation. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetActionTargetData(FBHWorldStateData TargetData);

	/** Return cost of use this ability to achieve DesiredWorldState; valid only if CanChangeWorldState returns true
	 * for the same parameters. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetActionCost(FBHWorldStateData DesiredWorldState, AActor* AgentActor,
		const TArray<FBHWorldStateData>& WithCurrentWorldState);

	/** Return true if action can be applicable on specified target actor and if possible effect world state. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetActionEffectWithContextActor(AActor* AgentActor, AActor* TargetActor, FBHWorldStateData& EffectWorldState);
	
};
