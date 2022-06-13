// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BHGOAPTypes.h"
#include "GameplayTagContainer.h"
#include "BHWorldStateAtom.generated.h"

/**
 * Object responsible for checking and returning one actor's parameter.
 */
UCLASS(Blueprintable, BlueprintType)
class BRAINHACKER_API UBHWorldStateAtom : public UObject
{
	GENERATED_BODY()

public:

	friend class UBHWorldStateProvider;

	/**
	 * Update world state atom data.
	 * @warning have to be implemented in all classes
	 * @warning have to update value of WorldStateValue
	 */
	UFUNCTION(BlueprintNativeEvent)
	void UpdateWorldStateAtomData();

protected:

	/** Tag which identify which type of data is this atom return. */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WorldStateAtomTag;

	/** Reference to actor for which this atom checks the state. */
	UPROPERTY(BlueprintReadOnly)
	AActor* OwnerActor = nullptr;

private:

	/** World state atom data, its can be data of any type. UBHWorldStateProvider used it to return specified type. */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FBHWorldStateValue WorldStateValue;
	
};
