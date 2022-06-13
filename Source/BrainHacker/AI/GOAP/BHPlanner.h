// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BHGOAPTypes.h"
#include "BHMemoryComponent.h"
#include "BrainHacker/GAS/BHAbilitySystemComponent.h"
#include "Components/ActorComponent.h"
#include "BHPlanner.generated.h"

class UBHSolver;
class UBHAction;
class UBHGoal;

/**
 * Helper struct for storing planned actions and theirs target data.
 */
USTRUCT()
struct FBHActionWithTargetData
{
	GENERATED_BODY()
	
	FBHActionWithTargetData() {}
	FBHActionWithTargetData(UBHAction* InAction, FBHWorldStateData InTargetData)
		: Action(InAction), TargetData(InTargetData) {}

	/** Planned action (ability). */
	UBHAction* Action = nullptr;
	/** Data required for action. Always is the same as desired world state data, which action needs achieve. */
	UPROPERTY()
	FBHWorldStateData TargetData;
};

/**
 * GOAP heart. Manage world state, goals and actions. Make decisions what goal will be considered and after
 * that how to achieve this goal. Planning sequence of few actions and execute them. Check validity of goals
 * and actions. Change current priority or way to achieve it if need.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRAINHACKER_API UBHPlanner : public UActorComponent
{
	GENERATED_BODY()

public:

	UBHPlanner();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Return reference to agent which is controlled by this planner. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* GetAgent() const { return GetOwner(); }
	/** Return agent's ability system component. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UBHAbilitySystemComponent* GetAgentsAbilitySystemComponent() const { return AgentsAbilitySystemComponent; }
	/** Return agent's ability system component. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UBHMemoryComponent* GetAgentsMemoryComponent() const { return AgentsMemoryComponent; }
	/** Return reference to current goal. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UBHGoal* GetPursuedGoal() const { return PursuedGoal; }

	/** Add goal to planner at runtime. */
	UFUNCTION(BlueprintCallable)
	void AddGoal(TSubclassOf<UBHGoal> GoalClass);
	/** Remove goal from planner at runtime. If removed goals is current pursued goal planner immediately change gaol. */
	UFUNCTION(BlueprintCallable)
	void RemoveGoal(TSubclassOf<UBHGoal> GoalClass);
	
protected:

	/** Searches all available goals and return goal with best score. */
	UBHGoal* FindBestScoredGoal();
	/** Set pursued goal, try build plan for it and execute. */
	void SetPursuedGoal(UBHGoal* Goal);
	/** Start performing given plan. Return true if successfully started. */
	bool ExecutePlan(TArray<FBHActionWithTargetData> Plan);
	/** Activate action of index ExecutingPlanActionIndex from ExecutingPlan. Return true if successfully activated. */
	bool ExecuteCurrentAction();
	/** Return true if any plan is currently executing. */
	bool IsCurrentlyExecutingPlan() const;
	/** Called to finish executing plan. */
	void FinishExecutePlan();

	/** Called when ability finished (properly or canceled). */
	UFUNCTION()
	void OnAbilityEnded(const FAbilityEndedData& Data);

	virtual void BeginPlay() override;

private:

	/** Reference to owner's (agent's) ability system component. */
	UBHAbilitySystemComponent* AgentsAbilitySystemComponent;
	/** Reference to owner's (agent's) memory component. */
	UBHMemoryComponent* AgentsMemoryComponent;

	/** Solver which will be used to finding plan. */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBHSolver> SolverImplementationClass;
	/** Reference to solver, which is created on BeginPlay by planner. */
	UPROPERTY()
	UBHSolver* Solver;

	/** List of potential goals, that planner can consider. */
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UBHGoal>> GoalsClasses;

	/** List of actual available goals. */
	UPROPERTY()
	TArray<UBHGoal*> Goals;

	/** Currently realised goal. */
	UPROPERTY()
	UBHGoal* PursuedGoal = nullptr;

	/** If currently is executing plan, here is current action index; -1 otherwise. */
	int32 ExecutingPlanActionIndex = -1;
	/** Currently realized plan. Can be empty if isn't realized any plan. */
	UPROPERTY()
	TArray<FBHActionWithTargetData> ExecutingPlan;

	/** Delegate handle for ability end. Can also be used to check if currently is realizing any action. */
	FDelegateHandle CurrentActionHandle;
};