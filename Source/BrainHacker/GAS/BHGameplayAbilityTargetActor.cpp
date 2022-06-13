// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/GAS/BHGameplayAbilityTargetActor.h"

void ABHGameplayAbilityTargetActor::ConfirmTarget()
{
	ConfirmTargeting();
}

void ABHGameplayAbilityTargetActor::CancelTarget()
{
	CancelTargeting();
}

void ABHGameplayAbilityTargetActor::OnCancelInput_Implementation()
{
}

void ABHGameplayAbilityTargetActor::OnConfirmInput_Implementation()
{
}
