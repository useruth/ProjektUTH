// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/AI/BHAIFunctionLibrary.h"


UBHPlanner* UBHAIFunctionLibrary::GetGOAPPlanner(const TScriptInterface<IBHGOAPAgent>& ForAgent)
{
	return ForAgent ? ForAgent->GetGOAPPlanner() : nullptr;
}

UBHMemoryComponent* UBHAIFunctionLibrary::GetGOAPMemoryComponent(const TScriptInterface<IBHGOAPAgent>& ForAgent)
{
	return ForAgent ? ForAgent->GetGOAPMemoryComponent() : nullptr;
}
