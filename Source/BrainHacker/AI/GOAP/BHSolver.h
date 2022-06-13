// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BHPlanner.h"
#include "UObject/NoExportTypes.h"
#include "BHSolver.generated.h"

class UBHGoal;

/**
 * Solver for finding solution (plan) for specified problem (goal). Used by BHPlanner.
 */
UCLASS(Abstract)
class BRAINHACKER_API UBHSolver : public UObject
{
	GENERATED_BODY()

public:

	/** Prepare solver to works for specified planner. */
	virtual void InitializeSolver(UBHPlanner* ForPlanner);

	/** Return complete plan for specified goal. Can return empty array if goal can't be satisfied. */
	virtual TArray<FBHActionWithTargetData> FindPlanForGoal(UBHGoal* Goal);

protected:

	/** Reference to planner for which this solver is working. */
	UBHPlanner* Planner;
	
};