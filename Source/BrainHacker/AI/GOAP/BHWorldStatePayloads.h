// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BHWorldStatePayloads.generated.h"

/**
 * Base class for BHWorldStateValue payload which contain data. Inherit from this class to create specified value type.
 */
UCLASS(Abstract)
class BRAINHACKER_API UBHWorldStatePayload : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Set payload value form pointer to void.
	 * @warning not check if pointer points to valid data.
	 */
	virtual void SetPayloadValueFromRawData(void* DataValueData) PURE_VIRTUAL(UBHWorldStatePayload::SetValueFromRawData);
	/** Return true if given FProperty points to data of type which can be stored in payload. */
	virtual bool IsValidPropertyForPayload(FProperty* DataProperty) PURE_VIRTUAL(UBHWorldStatePayload::IsValidProperty, return false;);
};

/**
 * Payload to store a bool variable.
 */
UCLASS()
class BRAINHACKER_API UBHWorldStatePayloadBool : public UBHWorldStatePayload
{
	GENERATED_BODY()

public:

	bool Value;

	virtual void SetPayloadValueFromRawData(void* DataValueData) override
	{
		Value = *(static_cast<bool*>(DataValueData));
	}
	virtual bool IsValidPropertyForPayload(FProperty* DataProperty) override
	{
		return CastField<FBoolProperty>(DataProperty) != nullptr;
	}
};

/**
 * Payload to store a int32 variable.
 */
UCLASS()
class BRAINHACKER_API UBHWorldStatePayloadInt : public UBHWorldStatePayload
{
	GENERATED_BODY()

public:

	int32 Value;

	virtual void SetPayloadValueFromRawData(void* DataValueData) override
	{
		Value = *(static_cast<int32*>(DataValueData));
	}
	virtual bool IsValidPropertyForPayload(FProperty* DataProperty) override
	{
		return CastField<FIntProperty>(DataProperty) != nullptr;
	}
};

/**
 * Payload to store a float variable.
 * @warning: in fact this payload is for double variable in UE5 and real float in UE4. this because in UE5 in BP float
 * variable is really of type double.
 */
UCLASS()
class BRAINHACKER_API UBHWorldStatePayloadFloat : public UBHWorldStatePayload
{
	GENERATED_BODY()
	
public:

#if ENGINE_MAJOR_VERSION >= 5 // UE5
	
	double Value;

	virtual void SetPayloadValueFromRawData(void* DataValueData) override
	{
		Value = *(static_cast<double*>(DataValueData));
	}
	virtual bool IsValidPropertyForPayload(FProperty* DataProperty) override
	{
		return CastField<FDoubleProperty>(DataProperty) != nullptr;
	}
	
#else // UE4
	
    float Value;
    
    virtual void SetPayloadValueFromRawData(void* DataValueData) override
    {
    	Value = *(static_cast<float*>(DataValueData));
    }
    virtual bool IsValidPropertyForPayload(FProperty* DataProperty) override
    {
    	return CastField<FFloatProperty>(DataProperty) != nullptr;
    }

#endif
};

/**
 * Payload to store a FVector variable.
 */
UCLASS()
class BRAINHACKER_API UBHWorldStatePayloadVector : public UBHWorldStatePayload
{
	GENERATED_BODY()
	
public:

	FVector Value;

	virtual void SetPayloadValueFromRawData(void* DataValueData) override
	{
		Value = *(static_cast<FVector*>(DataValueData));
	}
	virtual bool IsValidPropertyForPayload(FProperty* DataProperty) override
	{
		return DataProperty->GetCPPType().Equals("FVector");
	}
};

/**
 * Payload to store a FString variable.
 */
UCLASS()
class BRAINHACKER_API UBHWorldStatePayloadString : public UBHWorldStatePayload
{
	GENERATED_BODY()
	
public:

	FString Value;

	virtual void SetPayloadValueFromRawData(void* DataValueData) override
	{
		Value = *(static_cast<FString*>(DataValueData));
	}
	virtual bool IsValidPropertyForPayload(FProperty* DataProperty) override
	{
		return CastField<FStrProperty>(DataProperty) != nullptr;
	}
};

/**
 * Payload to store a AActor reference.
 */
UCLASS()
class BRAINHACKER_API UBHWorldStatePayloadActor : public UBHWorldStatePayload
{
	GENERATED_BODY()
	
public:

	AActor* Value;

	virtual void SetPayloadValueFromRawData(void* DataValueData) override
	{
		Value = *(static_cast<AActor**>(DataValueData));
	}
	virtual bool IsValidPropertyForPayload(FProperty* DataProperty) override
	{
		const FObjectProperty* ObjectProperty = CastField<FObjectProperty>(DataProperty);
		if(ObjectProperty)
		{
			// prefix 'A' has only actor, objects which isn't actor has 'U' prefix
			return FString(ObjectProperty->PropertyClass->GetPrefixCPP()).Equals("A");
		}
		return false;
	}
};