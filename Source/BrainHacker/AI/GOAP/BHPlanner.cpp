// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/AI/GOAP/BHPlanner.h"

#include "BHAction.h"
#include "BHGOAPAgent.h"
#include "BHSolver.h"
#include "BHWorldStateFunctionLibrary.h"
#include "BrainHacker/AI/GOAP/BHGoal.h"

UBHPlanner::UBHPlanner()
{
	// planner tick settings - init to tick tow times per frame; on each tick goal will be validated and new plan
	// will be generated if required
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.5f;
}

void UBHPlanner::BeginPlay()
{
	Super::BeginPlay();

	// get reference to ability system component
	AgentsAbilitySystemComponent = Cast<UBHAbilitySystemComponent>(GetOwner()->GetComponentByClass(UBHAbilitySystemComponent::StaticClass()));
	ensureMsgf(AgentsAbilitySystemComponent, TEXT("BHPlanner's owner need to has BHAbilitySystemComponent!"));
	// get reference to memory component
	AgentsMemoryComponent = Cast<UBHMemoryComponent>(GetOwner()->GetComponentByClass(UBHMemoryComponent::StaticClass()));
	ensureMsgf(AgentsMemoryComponent, TEXT("BHPlanner's owner need to has UBHMemoryComponent!"));

	// create solver
	Solver = NewObject<UBHSolver>(this, SolverImplementationClass);
	ensureMsgf(Solver, TEXT("BHPlanner can't create solver of given class!"));
	Solver->InitializeSolver(this);
	
	// create goals objects
	for(auto GoalClass : GoalsClasses)
	{
		if(!IsValid(GoalClass))
			continue;
	
		UBHGoal* NewGoal = NewObject<UBHGoal>(this, GoalClass);
		if(!NewGoal)
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create Goal of class %s!"), *GoalClass->GetName());
			continue;
		}
	
		NewGoal->AgentActor = TScriptInterface<IBHGOAPAgent>(GetOwner());
		Goals.Add(NewGoal);
	}
}

void UBHPlanner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// @warning: by default it not tick every frame (and shouldn't tick every frame) - see constructor
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// validate if current goal is best goal on each tick
	UBHGoal* CurrentBestGoal = FindBestScoredGoal();
	if(PursuedGoal != CurrentBestGoal)
	{
		TArray<UGameplayAbility*> ActiveAbilities = AgentsAbilitySystemComponent->GetActiveAbilities();
		if(ActiveAbilities.Num() > 0)
		{
			// assume that only one ability can be active at a time
			const UGameplayAbility* Ability = ActiveAbilities[0];
			// can't cancel ability so can't change goal at this moment
			if(!Ability->CanBeCanceled())
				return;
			// cancel ability if possible to switch goal; must be CDO object
			AgentsAbilitySystemComponent->CancelAbility(Cast<UGameplayAbility>(Ability->GetClass()->ClassDefaultObject));
		}
		// switch to other goal
		SetPursuedGoal(CurrentBestGoal);
	}
}

void UBHPlanner::AddGoal(TSubclassOf<UBHGoal> GoalClass)
{
	if(!IsValid(GoalClass))
		return;
	
	// check if this goal is not already in the array
	const int32 GoalIndex = Goals.IndexOfByPredicate([GoalClass](const UBHGoal* Goal)
	{
		return Goal->GetClass() == GoalClass;
	});
	
	if(GoalIndex != INDEX_NONE)
		return;
	
	UBHGoal* NewGoal = NewObject<UBHGoal>(this, GoalClass);
	if(!NewGoal)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant create Goal of class %s!"), *GoalClass->GetName());
		return;
	}
	
	NewGoal->AgentActor = TScriptInterface<IBHGOAPAgent>(GetOwner());
	Goals.Add(NewGoal);
}

void UBHPlanner::RemoveGoal(TSubclassOf<UBHGoal> GoalClass)
{
	if(!IsValid(GoalClass))
		return;

	// find goal
	const int32 GoalIndex = Goals.IndexOfByPredicate([GoalClass](const UBHGoal* Goal)
	{
		return Goal->GetClass() == GoalClass;
	});

	if(GoalIndex == INDEX_NONE)
		return;

	if(GetPursuedGoal() == Goals[GoalIndex])
	{
		// current goal is goal to remove
		TArray<UGameplayAbility*> ActiveAbilities = AgentsAbilitySystemComponent->GetActiveAbilities();
		if(ActiveAbilities.Num() > 0)
		{
			// assume that only one ability can be active at a time
			const UGameplayAbility* Ability = ActiveAbilities[0];
			// cancel ability if possible to switch goal; must be CDO object
			AgentsAbilitySystemComponent->CancelAbility(Cast<UGameplayAbility>(Ability->GetClass()->ClassDefaultObject));
			// reset pursued goal to force the planner to find another plan
			PursuedGoal = nullptr;
		}
	}

	Goals.RemoveAt(GoalIndex);
}

UBHGoal* UBHPlanner::FindBestScoredGoal()
{
	float BestScore = 0.0f;
	UBHGoal* BestGoal = nullptr;
	for(const auto Goal : Goals)
	{
		if(!Goal->IsGoalValid())
			continue;
			
		const float GoalScore = Goal->GetGoalScore();
		if(GoalScore > BestScore)
		{
			BestScore = GoalScore;
			BestGoal = Goal;
		}
	}
	return BestGoal;
}

void UBHPlanner::SetPursuedGoal(UBHGoal* Goal)
{
	PursuedGoal = Goal;
	TArray<FBHActionWithTargetData> Plan = Solver->FindPlanForGoal(PursuedGoal);
	if(Plan.Num() > 0)
	{
		// info log
		UE_LOG(LogTemp, Log, TEXT("New goal set: %s"), *PursuedGoal->GetName());
		for(auto Action : Plan)
		{
			FString ActionString = Action.Action->GetName();
			if(UBHWorldStateFunctionLibrary::IsWorldStateValueOfTypeActor(Action.TargetData.WorldStateValue))
			{
				const AActor* ContextActor =
					UBHWorldStateFunctionLibrary::GetWorldStateValueAsActor(Action.TargetData.WorldStateValue);
				if(ContextActor)
				{
					ActionString += "[" + ContextActor->GetName() + "]";
				}
			}
			UE_LOG(LogTemp, Log, TEXT("	- %s"), *ActionString);
		}
		// execute plan
		const bool ExecutionBegun = ExecutePlan(Plan);
		if(ExecutionBegun)
		{
			UE_LOG(LogTemp, Log, TEXT("Execution started!"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Execution failed!"));
		}
	}
}

bool UBHPlanner::ExecutePlan(TArray<FBHActionWithTargetData> Plan)
{
	if(Plan.Num() == 0 )
		return false;

	ExecutingPlan = Plan;
	ExecutingPlanActionIndex = 0;
	CurrentActionHandle.Reset();

	return ExecuteCurrentAction();
}

bool UBHPlanner::ExecuteCurrentAction()
{
	// can't activate action if another isn't finished
	if(CurrentActionHandle.IsValid())
		return false;
	
	CurrentActionHandle = AgentsAbilitySystemComponent->OnAbilityEnded.AddUObject(this, &UBHPlanner::OnAbilityEnded);

	// set target data for ability before activation
	ExecutingPlan[ExecutingPlanActionIndex].Action->SetActionTargetData(ExecutingPlan[ExecutingPlanActionIndex].TargetData);
	const bool ActivationSuccess =
		AgentsAbilitySystemComponent->TryActivateAbilityByClass(ExecutingPlan[ExecutingPlanActionIndex].Action->GetClass());
	if(!ActivationSuccess)
		return false;

	return true;
}

void UBHPlanner::OnAbilityEnded(const FAbilityEndedData& Data)
{
	AgentsAbilitySystemComponent->OnAbilityEnded.Remove(CurrentActionHandle);
	CurrentActionHandle.Reset();
	
	if(Data.bWasCancelled)
	{
		FinishExecutePlan();
		return;
	}
	
	if(ExecutingPlanActionIndex == ExecutingPlan.Num()-1)
	{
		FinishExecutePlan();
	}
	else
	{
		++ExecutingPlanActionIndex;
		const bool ActivationSuccess = ExecuteCurrentAction();
		if(!ActivationSuccess)
			FinishExecutePlan();
	}
}

bool UBHPlanner::IsCurrentlyExecutingPlan() const
{
	return ExecutingPlan.IsValidIndex(ExecutingPlanActionIndex);
}

void UBHPlanner::FinishExecutePlan()
{
	ExecutingPlan.Reset();
	ExecutingPlanActionIndex = -1;
	
	PursuedGoal = nullptr;
}