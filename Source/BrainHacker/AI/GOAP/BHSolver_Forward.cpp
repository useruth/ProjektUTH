// Fill out your copyright notice in the Description page of Project Settings.


#include "BrainHacker/AI/GOAP/BHSolver_Forward.h"

#include "BHWorldStateFunctionLibrary.h"
#include "BrainHacker/AI/GOAP/BHGoal.h"

TArray<FBHActionWithTargetData> UBHSolver_Forward::FindPlanForGoal(UBHGoal* Goal)
{
	if(!Goal)
	{
		return TArray<FBHActionWithTargetData>();
	}

	UE_LOG(LogTemp, Log, TEXT("Start looking for solution for goal: %s (forward planning)"), *Goal->GetName());
	
	TArray<FBHTreeNode> KnownNodes;
	TArray<int32> AvailableNodes;
	int32 CurrentNodeIndex;

	FBHTreeNode InitNode;
	InitNode.CurrentWorldStates = TArray<FBHWorldStateData>();
	
	if(!UBHWorldStateFunctionLibrary::IsWorldStateActual(Goal->GetDesiredWorldState()))
	{
		InitNode.DesiredWorldStates.Add(Goal->GetDesiredWorldState());
	}

	InitNode.Cost = 0;
	InitNode.Heuristic = InitNode.DesiredWorldStates.Num();
	if(InitNode.IsGoalSatisfiedInNode())
	{
		// goal is satisfied without any actions
		return TArray<FBHActionWithTargetData>();
	}
	InitNode.PathToNode.Add(0);
	KnownNodes.Add(InitNode);
	CurrentNodeIndex = 0;

	ExpandNode(InitNode, KnownNodes, AvailableNodes);
	int32 VisitedNodesNum = 1;
	while(AvailableNodes.Num() > 0)
	{
		const int32 BestNodeIndex = FindBestNode(KnownNodes, AvailableNodes);
		CurrentNodeIndex = BestNodeIndex;
		if(KnownNodes[CurrentNodeIndex].IsGoalSatisfiedInNode())
			break;
		AvailableNodes.Remove(CurrentNodeIndex);
		ExpandNode(KnownNodes[CurrentNodeIndex], KnownNodes, AvailableNodes);
		
		UE_LOG(LogTemp, Log, TEXT("Visited nodes number: %d; current cost: %d; current heuristic: %d"), VisitedNodesNum,
			KnownNodes[CurrentNodeIndex].Cost, KnownNodes[CurrentNodeIndex].Heuristic);

		++VisitedNodesNum;
	}
	UE_LOG(LogTemp, Log, TEXT("Summary visited nodes number: %d"), VisitedNodesNum);
	
	return KnownNodes[CurrentNodeIndex].IsGoalSatisfiedInNode() ?
		BuildActionsPlanForPath(KnownNodes, KnownNodes[CurrentNodeIndex].PathToNode) : TArray<FBHActionWithTargetData>();
}

bool UBHSolver_Forward::ExpandNode(FBHTreeNode Node, TArray<FBHTreeNode>& KnownNodes, TArray<int32>& AvailableNodes)
{
	bool bAnyNodeAdded = false;
	// check all available actions on each context actor
	for(auto Action : Planner->GetAgentsAbilitySystemComponent()->GetActivatableBHActions())
	{
		for(auto ContextActor : Planner->GetAgentsMemoryComponent()->GetMemory())
		{
			FBHWorldStateData ActionEffect;
			if(Action->GetActionEffectWithContextActor(Planner->GetAgent(), ContextActor, ActionEffect))
			{
				// preconditions check
				TArray<FBHWorldStateData> ActionPreconditions = Action->GetWorldStatePreconditions(ActionEffect, Planner->GetAgent());
				bool bAllPreconditionsMet = true;
				for(auto& Precondition : ActionPreconditions)
				{
					if(!IsDesiredWorldStateSatisfiedForNode(Precondition, Node))
					{
						bAllPreconditionsMet = false;
						break;
					}
				}
				if(!bAllPreconditionsMet)
					continue;
				// action is applicable on context actor - prepare new node
				FBHTreeNode NewNode = Node;
				// direct action leading to this node and associated target data
				NewNode.DirectAction = Action;
				NewNode.DirectTargetData = ActionEffect;
				// current world state
				UBHWorldStateFunctionLibrary::AddDataToWorldStateArray(NewNode.CurrentWorldStates, ActionEffect);
				// cost
				NewNode.Cost += bUseSimplifiedActionCost ? 1 : Action->GetActionCost(ActionEffect, Planner->GetAgent(),
					NewNode.CurrentWorldStates);
				// desired states need to be iterated to check if is still valid
				for(int32 Index = NewNode.DesiredWorldStates.Num()-1; Index >= 0; --Index)
				{
					if(IsDesiredWorldStateSatisfiedForNode(NewNode.DesiredWorldStates[Index], NewNode))
					{
						NewNode.DesiredWorldStates.RemoveAt(Index);
					}
				}
				// calculate heuristic
				NewNode.Heuristic = NewNode.DesiredWorldStates.Num();
				// add to arrays and update path to node
				int32 NewNodeIndex = KnownNodes.Add(NewNode);
				KnownNodes[NewNodeIndex].PathToNode.Add(NewNodeIndex);
				AvailableNodes.Add(NewNodeIndex);
				bAnyNodeAdded = true;
			}
		}
	}
	return bAnyNodeAdded;
}

bool UBHSolver_Forward::IsDesiredWorldStateSatisfiedForNode(const FBHWorldStateData& DesiredWorldState, const FBHTreeNode& Node)
{
	// check if node's current world state satisfy desired world state
	for(auto& WorldState : Node.CurrentWorldStates)
	{
		if(DesiredWorldState.WorldStateKey == WorldState.WorldStateKey &&
			UBHWorldStateFunctionLibrary::AreWorldStateValuesEquals(DesiredWorldState.WorldStateValue, WorldState.WorldStateValue))
		{
			return true;
		}
	}
	// check if desired state is actual world state
	return UBHWorldStateFunctionLibrary::IsWorldStateActual(DesiredWorldState);
}

int32 UBHSolver_Forward::FindBestNode(const TArray<FBHTreeNode>& KnownNodes, const TArray<int32>& AvailableNodes)
{
	int32 BestIndex = -1;
	int32 MinFx = MAX_int32;
	for(auto NodeIndex : AvailableNodes)
	{
		const int32 NodeFx = KnownNodes[NodeIndex].GetNodeFx();
		if(NodeFx < MinFx)
		{
			BestIndex = NodeIndex;
			MinFx = NodeFx;
		}
	}
	return BestIndex;
}

TArray<FBHActionWithTargetData> UBHSolver_Forward::BuildActionsPlanForPath(const TArray<FBHTreeNode>& KnownNodes,
	TArray<int32> Path)
{
	TArray<FBHActionWithTargetData> Result;
	for(auto PathPoint : Path)
	{
		UBHAction* Action = KnownNodes[PathPoint].DirectAction;
		if(Action)
		{
			Result.Add(FBHActionWithTargetData(KnownNodes[PathPoint].DirectAction, KnownNodes[PathPoint].DirectTargetData));
		}
	}
	return Result;
}
