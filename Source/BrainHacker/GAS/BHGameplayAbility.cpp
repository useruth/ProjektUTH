// Fill out your copyright notice in the Description page of Project Settings.


#include "BHGameplayAbility.h"

UBHGameplayAbility::UBHGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

FGameplayTag UBHGameplayAbility::GetTriggerTag()
{
	for(auto Trigger : AbilityTriggers)
	{
		if(Trigger.TriggerSource == EGameplayAbilityTriggerSource::GameplayEvent)
		{
			return Trigger.TriggerTag;
		}
	}
	return FGameplayTag::EmptyTag;
}
