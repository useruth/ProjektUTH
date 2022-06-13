// Fill out your copyright notice in the Description page of Project Settings.


#include "BHAttributeSetBase.h"

UBHAttributeSetBase::UBHAttributeSetBase()
{
}

void UBHAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UBHAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
