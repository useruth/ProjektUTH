// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BHHUD.generated.h"

/**
 * Base HUD class.
 */
UCLASS()
class BRAINHACKER_API ABHHUD : public AHUD
{
	GENERATED_BODY()

public:

	/** Show end screen with possibility to restart game. */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowGameOverScreen();
	
};
