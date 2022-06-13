// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/AI/GOAP/BHGoal.h"

FBHWorldStateData UBHGoal::GetDesiredWorldState()
{
	// update desired world state to always return current data
	UpdateDesiredWorldState();
	
	return DesiredWorldState;
}

void UBHGoal::UpdateDesiredWorldState_Implementation()
{
	// have to be implemented
	//unimplemented();
}
