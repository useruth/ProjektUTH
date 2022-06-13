// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/GAS/BHGAT_ActorArray.h"

void ABHGAT_ActorArray::ConfirmTargetingAndContinue()
{
	const auto ReturnData = new FGameplayAbilityTargetData_ActorArray();
	for (TSoftObjectPtr<AActor>& Target : TargetedActors)
	{
		AActor* Actor = Target.Get();
		if (Actor == nullptr)
			continue;
		ReturnData->TargetActorArray.AddUnique(TWeakObjectPtr<AActor>(Actor));
	}
	FGameplayAbilityTargetDataHandle Handle(ReturnData);
	TargetDataReadyDelegate.Broadcast(Handle);
}
