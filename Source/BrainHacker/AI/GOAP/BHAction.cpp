// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/AI/GOAP/BHAction.h"


bool UBHAction::CanChangeWorldState_Implementation(FBHWorldStateData DesiredWorldState, AActor* AgentActor)
{
	return false;
}

TArray<FBHWorldStateData> UBHAction::GetWorldStatePreconditions_Implementation(FBHWorldStateData ForDesiredWorldState, AActor* AgentActor)
{
	return TArray<FBHWorldStateData>();
}

void UBHAction::SetActionTargetData_Implementation(FBHWorldStateData TargetData)
{
	return;
}

int32 UBHAction::GetActionCost_Implementation(FBHWorldStateData DesiredWorldState, AActor* AgentActor,
	const TArray<FBHWorldStateData>& WithCurrentWorldState)
{
	return 0;
}

bool UBHAction::GetActionEffectWithContextActor_Implementation(AActor* AgentActor, AActor* TargetActor,
	FBHWorldStateData& EffectWorldState)
{
	return false;
}
