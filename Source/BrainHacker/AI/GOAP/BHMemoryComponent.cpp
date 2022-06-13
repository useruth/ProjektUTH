// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/AI/GOAP/BHMemoryComponent.h"

void UBHMemoryComponent::RegisterActorInMemory(AActor* Actor)
{
	if(!Actor)
		return;

	Memory.AddUnique(Actor);
	OnMemoryChangedDelegate.Broadcast(Actor, true);
}

void UBHMemoryComponent::UnregisterActorFromMemory(AActor* Actor)
{
	Memory.Remove(Actor);
	OnMemoryChangedDelegate.Broadcast(Actor, false);
}

bool UBHMemoryComponent::IsActorInMemory(AActor* Actor) const
{
	return Memory.Contains(Actor);
}

