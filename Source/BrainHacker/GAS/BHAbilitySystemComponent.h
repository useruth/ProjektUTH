// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BrainHacker/AI/GOAP/BHAction.h"
#include "BHAbilitySystemComponent.generated.h"

/**
 * Ability system component to use in all classes of characters in BH
 */
UCLASS()
class BRAINHACKER_API UBHAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UBHAbilitySystemComponent();

	/** Blueprint callable version of GetActivatableAbilities. */
	UFUNCTION(BlueprintCallable)
	TArray<UGameplayAbility*> GetActivatableAbilitiesBP();

	/** Return all BHActions (GOAP abilities) available for component. */
	UFUNCTION(BlueprintCallable)
	TArray<UBHAction*> GetActivatableBHActions();

	/** Return all active abilities. */
	UFUNCTION(BlueprintCallable)
	TArray<UGameplayAbility*> GetActiveAbilities();

	/** Return true if there is any ability with given tags that can be currently activated. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool CanActivateAbilityWithTag(const FGameplayTagContainer& AbilityTagContainer) const;

	/** Return true if given ability class cen be currently activated. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool CanActivateAbilityClass(const TSubclassOf<UGameplayAbility> AbilityClass) const;
	
};
