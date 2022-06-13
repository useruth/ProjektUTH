// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "BHGOAPTypes.generated.h"

class UBHWorldStatePayload;

/**
 * Uniquely identifies world state data.
 */
USTRUCT(BlueprintType)
struct FBHWorldStateKey
{
	GENERATED_BODY()

	FBHWorldStateKey() {}

	FBHWorldStateKey(AActor* InWorldStateActor, FGameplayTag InWorldStateDataTag)
		: WorldStateActor(InWorldStateActor), WorldStateDataTag(InWorldStateDataTag) {}

	FBHWorldStateKey(const FBHWorldStateKey& Other)
	: FBHWorldStateKey(Other.WorldStateActor, Other.WorldStateDataTag) {}

	bool operator==(const FBHWorldStateKey& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FBHWorldStateKey& Other) const
	{
		return WorldStateActor == Other.WorldStateActor &&
			WorldStateDataTag.MatchesTagExact(Other.WorldStateDataTag);
	}

	/** Actor which has world state data. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	AActor* WorldStateActor = nullptr;

	/** World state data tag for WorldStateActor. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTag WorldStateDataTag;
	
};

/** FBHWorldStateKey needs has hash to be key in TMap. */
FORCEINLINE  uint32 GetTypeHash(const FBHWorldStateKey& Thing)
{
	return FCrc::MemCrc32(&Thing, sizeof(FBHWorldStateKey));
}

/**
 * Atom world state value. In BP accessible by UBHAIFunctionLibrary functions.
 */
USTRUCT(BlueprintType)
struct FBHWorldStateValue
{
	GENERATED_BODY()

	FBHWorldStateValue() : Payload(nullptr) {}
	
	explicit FBHWorldStateValue(UBHWorldStatePayload* InPayload)
		: Payload(InPayload) {}

	/** Payload object with value. */
	UPROPERTY()
	UBHWorldStatePayload* Payload;
};

/**
 * Complete world state data - key with value.
 */
USTRUCT(BlueprintType)
struct FBHWorldStateData
{
	GENERATED_BODY()

	FBHWorldStateData() {}
	FBHWorldStateData(FBHWorldStateKey Key, FBHWorldStateValue Value)
		: WorldStateKey(Key), WorldStateValue(Value) {}

	/** World state ID. */
	UPROPERTY(BlueprintReadWrite)
	FBHWorldStateKey WorldStateKey;
	/** World state value. */
	UPROPERTY(BlueprintReadWrite)
	FBHWorldStateValue WorldStateValue;
};

