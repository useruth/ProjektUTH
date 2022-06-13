// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BHGOAPTypes.h"
#include "BHWorldStateAtom.h"
#include "Components/ActorComponent.h"
#include "BHWorldStateProvider.generated.h"

/**
* Each actor which can be considered by planner need has this component. Contains all world state data associated
* with owner actor. Manage world state atoms.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRAINHACKER_API UBHWorldStateProvider : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBHWorldStateProvider();

	/** Return true if provider has specified data (has specified world state atom). */
	UFUNCTION(BlueprintCallable)
	bool HasWorldStateValue(const FGameplayTag WorldStateAtomTag);
	/** Allows get specified world state value. This value can be later casted to specified value by
	 * UBHAIFunctionLibrary functions. */
	UFUNCTION(BlueprintCallable)
	FBHWorldStateValue GetWorldStateValue(const FGameplayTag WorldStateAtomTag);
	
protected:
	
	virtual void BeginPlay() override;

private:

	/** Actor with this component will has only world state data which is specified in this array. */
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UBHWorldStateAtom>> WorldSateAtomsClasses;

	/** Instances of atoms to check world state. */
	UPROPERTY()
	TArray<UBHWorldStateAtom*> WorldSateAtoms;
	
};
