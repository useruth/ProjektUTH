// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/AI/GOAP/BHWorldStateProvider.h"

UBHWorldStateProvider::UBHWorldStateProvider()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBHWorldStateProvider::BeginPlay()
{
	Super::BeginPlay();

	// create atoms objects
	for(auto AtomClass : WorldSateAtomsClasses)
	{
		if(!IsValid(AtomClass))
			continue;

		UBHWorldStateAtom* NewAtom = NewObject<UBHWorldStateAtom>(this, AtomClass);
		if(!NewAtom)
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create World State Atom of class %s!"), *AtomClass->GetName());
			continue;
		}

		NewAtom->OwnerActor = GetOwner();
		WorldSateAtoms.Add(NewAtom);
		NewAtom->UpdateWorldStateAtomData();
	}
}

bool UBHWorldStateProvider::HasWorldStateValue(const FGameplayTag WorldStateAtomTag)
{
	return WorldSateAtoms.ContainsByPredicate([WorldStateAtomTag](const UBHWorldStateAtom* Atom)
	{
		return Atom->WorldStateAtomTag.MatchesTagExact(WorldStateAtomTag);
	});
}

FBHWorldStateValue UBHWorldStateProvider::GetWorldStateValue(const FGameplayTag WorldStateAtomTag)
{
	for(const auto Atom : WorldSateAtoms)
	{
		if(Atom->WorldStateAtomTag.MatchesTagExact(WorldStateAtomTag))
		{
			Atom->UpdateWorldStateAtomData();
			return Atom->WorldStateValue;
		}
	}
	return FBHWorldStateValue();
}