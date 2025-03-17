// Copyright Catness Game Studios. All Rights Reserved.

#pragma once

class UDynamicMesh;
class UQF_QuantumMeshComponent;

class FApplyMeshBooleanTask
{
public:
	FApplyMeshBooleanTask(
		UQF_QuantumMeshComponent* InComponent,
		bool InbSubtract,
		UDynamicMesh* InTargetMesh)
		: Component(InComponent), 
		bSubtract(InbSubtract),
		TargetMesh(InTargetMesh)
	{
	}

private:
	UQF_QuantumMeshComponent* Component;
	bool bSubtract{false};
	UDynamicMesh* TargetMesh;

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
