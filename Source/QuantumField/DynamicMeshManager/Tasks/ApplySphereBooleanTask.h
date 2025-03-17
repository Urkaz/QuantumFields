// Copyright Catness Game Studios. All Rights Reserved.

#pragma once
#include "QuantumField/DynamicMeshManager/DynamicSphereManager.h"
#include "QuantumField/DynamicMeshManager/Data/QF_BooleanModifier.h"

class UDynamicMesh;

class FApplySphereBooleanTask
{
public:
	FApplySphereBooleanTask(UDynamicSphereManager* InManagerPtr, UDynamicMesh* InDynamicMes, TArray<FQF_BooleanModifier> InModifiers) : DynamicMesh(InDynamicMes), Modifiers(InModifiers), ManagerPtr(InManagerPtr)
	{
		
	}

private:
	UDynamicMesh* DynamicMesh;
	TArray<FQF_BooleanModifier> Modifiers;
	UDynamicSphereManager* ManagerPtr;

public:
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent);

	ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::AnyNormalThreadNormalTask;
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FUpdateDistanceTask, STATGROUP_TaskGraphTasks);
	}

	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}
};
