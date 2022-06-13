// Fill out your copyright notice in the Description page of Project Settings.


#include "BHGameInstance.h"
#include "AbilitySystemGlobals.h"

void UBHGameInstance::Init()
{
	Super::Init();
	
	// need be called to use TargetData
	UAbilitySystemGlobals::Get().InitGlobalData();
}
