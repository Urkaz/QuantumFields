// Copyright Catness Game Studios. All Rights Reserved.

#pragma once
#include "QuantumField/Actors/QuantumSphere.h"
#include "QuantumField/DynamicMeshManager/Interfaces/QF_IDynamicMeshManaged.h"


class FNotifyEndTask
{
public:
	FNotifyEndTask(
		AQuantumSphere* InSphere,
		TArray<IQF_IDynamicMeshManaged*> InAffectedComponents,
		bool InIsActivate
		)
	: Sphere(InSphere), AffectedComponents(InAffectedComponents), IsActivate(InIsActivate)
	{
	}

private:
	AQuantumSphere* Sphere;
	TArray<IQF_IDynamicMeshManaged*> AffectedComponents;
	bool IsActivate;

public:
	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent);

	ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::GameThread;
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
