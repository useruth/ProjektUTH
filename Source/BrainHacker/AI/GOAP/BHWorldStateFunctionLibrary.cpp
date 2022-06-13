// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/AI/GOAP/BHWorldStateFunctionLibrary.h"

#include "BHWorldStateProvider.h"

bool UBHWorldStateFunctionLibrary::GetWorldStateValueAsBool(const FBHWorldStateValue& Value)
{
	return Cast<UBHWorldStatePayloadBool>(Value.Payload)->Value;
}

int32 UBHWorldStateFunctionLibrary::GetWorldStateValueAsInt(const FBHWorldStateValue& Value)
{
	return Cast<UBHWorldStatePayloadInt>(Value.Payload)->Value;
}

float UBHWorldStateFunctionLibrary::GetWorldStateValueAsFloat(const FBHWorldStateValue& Value)
{
	return Cast<UBHWorldStatePayloadFloat>(Value.Payload)->Value;
}

FVector UBHWorldStateFunctionLibrary::GetWorldStateValueAsVector(const FBHWorldStateValue& Value)
{
	return Cast<UBHWorldStatePayloadVector>(Value.Payload)->Value;
}

FString UBHWorldStateFunctionLibrary::GetWorldStateValueAsString(const FBHWorldStateValue& Value)
{
	return Cast<UBHWorldStatePayloadString>(Value.Payload)->Value;
}

AActor* UBHWorldStateFunctionLibrary::GetWorldStateValueAsActor(const FBHWorldStateValue& Value)
{
	return Cast<UBHWorldStatePayloadActor>(Value.Payload)->Value;
}

bool UBHWorldStateFunctionLibrary::IsWorldStateValueOfTypeBool(const FBHWorldStateValue& Value)
{
	return (Cast<UBHWorldStatePayloadBool>(Value.Payload) != nullptr);
}

bool UBHWorldStateFunctionLibrary::IsWorldStateValueOfTypeInt(const FBHWorldStateValue& Value)
{
	return (Cast<UBHWorldStatePayloadInt>(Value.Payload) != nullptr);
}

bool UBHWorldStateFunctionLibrary::IsWorldStateValueOfTypeFloat(const FBHWorldStateValue& Value)
{
	return (Cast<UBHWorldStatePayloadFloat>(Value.Payload) != nullptr);
}

bool UBHWorldStateFunctionLibrary::IsWorldStateValueOfTypeVector(const FBHWorldStateValue& Value)
{
	return (Cast<UBHWorldStatePayloadVector>(Value.Payload) != nullptr);
}

bool UBHWorldStateFunctionLibrary::IsWorldStateValueOfTypeString(const FBHWorldStateValue& Value)
{
	return (Cast<UBHWorldStatePayloadString>(Value.Payload) != nullptr);
}

bool UBHWorldStateFunctionLibrary::IsWorldStateValueOfTypeActor(const FBHWorldStateValue& Value)
{
	return (Cast<UBHWorldStatePayloadActor>(Value.Payload) != nullptr);
}

bool UBHWorldStateFunctionLibrary::AreWorldStateValuesEquals(const FBHWorldStateValue& ValueOne, const FBHWorldStateValue& ValueTwo)
{
	if(IsWorldStateValueOfTypeBool(ValueOne) && IsWorldStateValueOfTypeBool(ValueTwo))
	{
		return (GetWorldStateValueAsBool(ValueOne) == GetWorldStateValueAsBool(ValueTwo));
	}
	if(IsWorldStateValueOfTypeInt(ValueOne) && IsWorldStateValueOfTypeInt(ValueTwo))
	{
		return (GetWorldStateValueAsInt(ValueOne) == GetWorldStateValueAsInt(ValueTwo));
	}
	if(IsWorldStateValueOfTypeFloat(ValueOne) && IsWorldStateValueOfTypeFloat(ValueTwo))
	{
		return (GetWorldStateValueAsFloat(ValueOne) == GetWorldStateValueAsFloat(ValueTwo));
	}
	if(IsWorldStateValueOfTypeVector(ValueOne) && IsWorldStateValueOfTypeVector(ValueTwo))
	{
		return (GetWorldStateValueAsVector(ValueOne) == GetWorldStateValueAsVector(ValueTwo));
	}
	if(IsWorldStateValueOfTypeString(ValueOne) && IsWorldStateValueOfTypeString(ValueTwo))
	{
		return (GetWorldStateValueAsString(ValueOne) == GetWorldStateValueAsString(ValueTwo));
	}
	if(IsWorldStateValueOfTypeActor(ValueOne) && IsWorldStateValueOfTypeActor(ValueTwo))
	{
		return (GetWorldStateValueAsActor(ValueOne) == GetWorldStateValueAsActor(ValueTwo));
	}

	return false;
}

void UBHWorldStateFunctionLibrary::AddDataToWorldStateArray(TArray<FBHWorldStateData>& WorldStateArray,
	const FBHWorldStateData& NewData)
{
	bool bWorldStateFound = false;
	for(auto& WorldState : WorldStateArray)
	{
		// if we already have value for this key - update it
		if(WorldState.WorldStateKey == NewData.WorldStateKey)
		{
			WorldState.WorldStateValue = NewData.WorldStateValue;
			bWorldStateFound = true;
		}
	}
	// new key
	if(!bWorldStateFound)
	{
		WorldStateArray.Add(NewData);
	}
}

bool UBHWorldStateFunctionLibrary::IsWorldStateActual(const FBHWorldStateData& DesiredWorldState)
{
	const AActor* WorldStateActor = DesiredWorldState.WorldStateKey.WorldStateActor;
	if(!WorldStateActor)
		return false;
	
	UBHWorldStateProvider* WorldStateProvider =
		Cast<UBHWorldStateProvider>(WorldStateActor->GetComponentByClass(UBHWorldStateProvider::StaticClass()));
	if(!WorldStateProvider)
		return false;

	const FBHWorldStateValue CurrentWorldState =
		WorldStateProvider->GetWorldStateValue(DesiredWorldState.WorldStateKey.WorldStateDataTag);

	return AreWorldStateValuesEquals(DesiredWorldState.WorldStateValue, CurrentWorldState);
}

FBHWorldStateValue UBHWorldStateFunctionLibrary::GetActualWorldStateValue(const FBHWorldStateKey& Key,
                                                                          const TArray<FBHWorldStateData>& WithCurrentWorldState)
{
	if(Key.WorldStateActor == nullptr || !Key.WorldStateDataTag.IsValid())
		return FBHWorldStateValue();
	
	for(auto CurrentWorldState : WithCurrentWorldState)
	{
		if(CurrentWorldState.WorldStateKey == Key)
		{
			return CurrentWorldState.WorldStateValue;
		}
	}

	const auto WorldStateProvider = Cast<UBHWorldStateProvider>(Key.WorldStateActor->GetComponentByClass(UBHWorldStateProvider::StaticClass()));
	if(!WorldStateProvider)
		return FBHWorldStateValue();
	
	if(!WorldStateProvider->HasWorldStateValue(Key.WorldStateDataTag))
		return FBHWorldStateValue();

	return  WorldStateProvider->GetWorldStateValue(Key.WorldStateDataTag);
}
