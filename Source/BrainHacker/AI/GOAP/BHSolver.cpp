// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/AI/GOAP/BHSolver.h"

void UBHSolver::InitializeSolver(UBHPlanner* ForPlanner)
{
	Planner = ForPlanner;
}

TArray<FBHActionWithTargetData> UBHSolver::FindPlanForGoal(UBHGoal* Goal)
{
	return TArray<FBHActionWithTargetData>();
}

