// Fill out your copyright notice in the Description page of Project Settings.


#include "BHAbilitySystemComponent.h"


UBHAbilitySystemComponent::UBHAbilitySystemComponent()
{
}

TArray<UGameplayAbility*> UBHAbilitySystemComponent::GetActivatableAbilitiesBP()
{
	TArray<FGameplayAbilitySpec> AbilitiesSpec = GetActivatableAbilities();
	TArray<UGameplayAbility*> Abilities;
	for(auto AbilitySpec : AbilitiesSpec)
	{
		Abilities.Add(AbilitySpec.GetAbilityInstances()[0]);
	}
	return Abilities;
}

TArray<UBHAction*> UBHAbilitySystemComponent::GetActivatableBHActions()
{
	TArray<UGameplayAbility*> Abilities = GetActivatableAbilitiesBP();
	TArray<UBHAction*> Actions;
	for(auto Ability : Abilities)
	{
		UBHAction* Action = Cast<UBHAction>(Ability);
		if(Action)
			Actions.Add(Action);
	}
	return Actions;
}

TArray<UGameplayAbility*> UBHAbilitySystemComponent::GetActiveAbilities()
{
	TArray<UGameplayAbility*> ActiveAbilities;
	TArray<UGameplayAbility*> Abilities = GetActivatableAbilitiesBP();
	for(auto Ability : Abilities)
	{
		if(Ability->IsActive())
			ActiveAbilities.Add(Ability);
	}
	return ActiveAbilities;
}

bool UBHAbilitySystemComponent::CanActivateAbilityWithTag(const FGameplayTagContainer& AbilityTagContainer) const
{
	TArray<FGameplayAbilitySpec*> AbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagContainer, AbilitySpecs);
	for (auto AbilitySpec : AbilitySpecs)
	{
		if (AbilitySpec->Ability->CanActivateAbility(AbilitySpec->Handle, AbilityActorInfo.Get()))
		{
			return true;
		}
	}
	return false;
}

bool UBHAbilitySystemComponent::CanActivateAbilityClass(const TSubclassOf<UGameplayAbility> AbilityClass) const
{
	if(!IsValid(AbilityClass))
	{
		return false;
	}
	const UGameplayAbility* AbilityCDO = AbilityClass.GetDefaultObject();
	const bool bIsNotCooldown = AbilityCDO->CheckCooldown(FGameplayAbilitySpecHandle(), AbilityActorInfo.Get());	
	const bool bCanActivate = CanActivateAbilityWithTag(AbilityClass.GetDefaultObject()->AbilityTags);
	return bIsNotCooldown && bCanActivate;
}
