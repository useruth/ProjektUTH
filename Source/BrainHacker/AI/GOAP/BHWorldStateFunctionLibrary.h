// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BHGOAPTypes.h"
#include "BHWorldStatePayloads.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BHWorldStateFunctionLibrary.generated.h"

/**
 * Utilities for world state.
 */
UCLASS()
class BRAINHACKER_API UBHWorldStateFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Most common functions for getting world state data. Can be also used in BP. */
	UFUNCTION(BlueprintCallable)
	static bool GetWorldStateValueAsBool(const FBHWorldStateValue& Value);
	UFUNCTION(BlueprintCallable)
	static int32 GetWorldStateValueAsInt(const FBHWorldStateValue& Value);
	UFUNCTION(BlueprintCallable)
	static float GetWorldStateValueAsFloat(const FBHWorldStateValue& Value);
	UFUNCTION(BlueprintCallable)
	static FVector GetWorldStateValueAsVector(const FBHWorldStateValue& Value);
	UFUNCTION(BlueprintCallable)
	static FString GetWorldStateValueAsString(const FBHWorldStateValue& Value);
	UFUNCTION(BlueprintCallable)
	static AActor* GetWorldStateValueAsActor(const FBHWorldStateValue& Value);
	/** Functions for checking type of world state value. Can be also used in BP. */
	UFUNCTION(BlueprintCallable)
	static bool IsWorldStateValueOfTypeBool(const FBHWorldStateValue& Value);
	UFUNCTION(BlueprintCallable)
	static bool IsWorldStateValueOfTypeInt(const FBHWorldStateValue& Value);
	UFUNCTION(BlueprintCallable)
	static bool IsWorldStateValueOfTypeFloat(const FBHWorldStateValue& Value);
	UFUNCTION(BlueprintCallable)
	static bool IsWorldStateValueOfTypeVector(const FBHWorldStateValue& Value);
	UFUNCTION(BlueprintCallable)
	static bool IsWorldStateValueOfTypeString(const FBHWorldStateValue& Value);
	UFUNCTION(BlueprintCallable)
	static bool IsWorldStateValueOfTypeActor(const FBHWorldStateValue& Value);
	/** Check if two world state values are identical. */
	UFUNCTION(BlueprintCallable)
	static bool AreWorldStateValuesEquals(const FBHWorldStateValue& ValueOne, const FBHWorldStateValue& ValueTwo);

	/**
	 * Add world state data to world state array. If value of specified key is already in array its value will
	 * be override. If there isn't this key - will be added.
	 */
	UFUNCTION(BlueprintCallable)
	static void AddDataToWorldStateArray(UPARAM(ref) TArray<FBHWorldStateData>& WorldStateArray, const FBHWorldStateData& NewData);
	/** Return true if DesiredWorldState is currently met. */
	UFUNCTION(BlueprintCallable)
	static bool IsWorldStateActual(const FBHWorldStateData& DesiredWorldState);
	/**
	 * Return world state value for given key. If this key is in WithCurrentWorldState it return value from this array,
	 * otherwise return actual (current) world state value. 
	 */
	UFUNCTION(BlueprintCallable)
	static FBHWorldStateValue GetActualWorldStateValue(const FBHWorldStateKey& Key,
		const TArray<FBHWorldStateData>& WithCurrentWorldState);

	/** Allow set data in FBHWorldStateValue variable in BP. */
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "Data"))
	static void SetWorldStateValue(FBHWorldStateValue Target, UStruct* Data, FBHWorldStateValue& Result);

	DECLARE_FUNCTION(execSetWorldStateValue)
	{
		// Steps into the stack, walking to the next property in it
		Stack.Step(Stack.Object, nullptr);
		// Prepare FBHWorldStateValue Target
		FProperty* TargetVariableProperty = CastField<FProperty>(Stack.MostRecentProperty);
		void* TargetVariableData = Stack.MostRecentPropertyAddress;
		FBHWorldStateValue* TargetVariable = static_cast<FBHWorldStateValue*>(TargetVariableData);

		// Steps into the stack, walking to the next property in it
		Stack.Step(Stack.Object, nullptr);
		// Prepare UStruct* Data
		FProperty* DataProperty = CastField<FProperty>(Stack.MostRecentProperty);
		void* DataValueData = Stack.MostRecentPropertyAddress;

		// Get all payload classes
		TArray<UClass*> PayloadsTypes;
		GetDerivedClasses(UBHWorldStatePayload::StaticClass(), PayloadsTypes);
		// Find proper payload and set value
		for(auto Type : PayloadsTypes)
		{
			if(Cast<UBHWorldStatePayload>(Type->GetDefaultObject())->IsValidPropertyForPayload(DataProperty))
			{
				// Payload object of proper type is already crated - simply override value
				if(TargetVariable->Payload && TargetVariable->Payload->GetClass() == Type)
				{
					TargetVariable->Payload->SetPayloadValueFromRawData(DataValueData);
				}
				// There isn't payload object or is of other type - create new payload and set value
				else
				{
					UBHWorldStatePayload* Payload = NewObject<UBHWorldStatePayload>(GetTransientPackage(), Type);
					Payload->SetPayloadValueFromRawData(DataValueData);
					TargetVariable->Payload = Payload;
				}
				break;
			}
		}

		// Steps into the stack, walking to the next property in it
		Stack.Step(Stack.Object, nullptr);
		// Prepare FBHWorldStateValue TargetVariable
		FProperty* ResultProperty = CastField<FProperty>(Stack.MostRecentProperty);
		void* ResultData = Stack.MostRecentPropertyAddress;
		FBHWorldStateValue* ResultVariable = static_cast<FBHWorldStateValue*>(ResultData);
		*ResultVariable = *TargetVariable;
		
		// We need this to wrap up the stack
		P_FINISH;

		// Return value which was set
		*(FBHWorldStateValue*)RESULT_PARAM = *ResultVariable;
	}
};
