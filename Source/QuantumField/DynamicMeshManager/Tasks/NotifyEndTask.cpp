#include "NotifyEndTask.h"

void FNotifyEndTask::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("FNotifyEndTask FINISHED"));

	if (IsActivate)
	{
		Sphere->TryNotifyEndActivate();
	}
	else
	{
		Sphere->TryNotifyEndDeactivate();
	}
	// for (IQF_IDynamicMeshManaged* Component : AffectedComponents)
	// {
	// 	Component->NotifyEndTask();1
	// }
}
