// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrainHacker/AI/GOAP/BHSolver.h"
#include "BHSolver_Forward.generated.h"

/**
 * Forward planning implementation.
 */
UCLASS()
class BRAINHACKER_API UBHSolver_Forward : public UBHSolver
{
	GENERATED_BODY()
	
public:

	/** Return complete plan for specified goal. Can return empty array if goal can't be satisfied. */
	virtual TArray<FBHActionWithTargetData> FindPlanForGoal(UBHGoal* Goal) override;

private:

	/**
	 * If true solver will be use cost = 1 for each action. Action's original cost is ignored, but thanks that
	 * forward planning is more efficient.
	 */
	bool bUseSimplifiedActionCost = true;

	/**
	 * Helper struct representing node of decision tree.
	 */
	struct FBHTreeNode
	{
		/** Indexes leading to this node (parents). */
		TArray<int32> PathToNode;
		/** Action immediately preceding this node (action to be performed to get to this node from the parent). */
		UBHAction* DirectAction = nullptr;
		/** Target data for DirectAction. */
		FBHWorldStateData DirectTargetData;

		/** Current world state in this node (contains only values which was changed by this node or parents nodes). */
		TArray<FBHWorldStateData> CurrentWorldStates;
		/** Desired world state for this node. */
		TArray<FBHWorldStateData> DesiredWorldStates;
		/** Total cost of the need to reach this node from init node. */
		int32 Cost = 0;
		/** Heuristic (for A*) which is equal to number of desired states. */
		int32 Heuristic = 0;

		/** Return true if this node satisfy goal (all desired states are met). */
		bool IsGoalSatisfiedInNode() const { return Heuristic == 0; }
		/** Return f(x) value for A* (f(x)=g(x)+h(x)). */
		int32 GetNodeFx() const { return Cost + Heuristic; }
	};

	/** Find all possible actions to perform in Node, create nodes from them and add to KnownNodes and AvailableNodes. */
	bool ExpandNode(FBHTreeNode Node, TArray<FBHTreeNode>& KnownNodes, TArray<int32>& AvailableNodes);
	/**
	 * Return true if DesiredWorldState is met in given Node (the state was satisfied by some planned actions or
	 * this state is actual state and not changed by any planned action).
	 */
	bool IsDesiredWorldStateSatisfiedForNode(const FBHWorldStateData& DesiredWorldState, const FBHTreeNode& Node);
	/** Return index of best node (node of min value of A* f(x)=g(x)+h(x)). */
	int32 FindBestNode(const TArray<FBHTreeNode>& KnownNodes, const TArray<int32>& AvailableNodes);
	/** Return list of actions from given indexes (Path) and nodes (KnownNodes). */
	TArray<FBHActionWithTargetData> BuildActionsPlanForPath(const TArray<FBHTreeNode>& KnownNodes, TArray<int32> Path);
};
