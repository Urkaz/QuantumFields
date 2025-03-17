#include "ApplyMeshBooleanTask.h"

#include "GeometryScript/MeshBooleanFunctions.h"
#include "QuantumField/DynamicMeshManager/DynamicSphereManager.h"

void FApplyMeshBooleanTask::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	const UDynamicSphereManager* DynMeshMgr = UDynamicSphereManager::Get(Component);
	if (DynMeshMgr->GetSpheresBoolean())
	{
		FGeometryScriptMeshBooleanOptions Obj;
		Obj.bSimplifyOutput = false;
		UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean(TargetMesh,
																			 Component->GetComponentTransform(),
																			 DynMeshMgr->GetSpheresBoolean(),
																			 FTransform(),
																			 bSubtract ? EGeometryScriptBooleanOperation::Subtract : EGeometryScriptBooleanOperation::Intersection,
																			 Obj);
	}
	//UE_LOG(LogTemp, Warning, TEXT("FApplyMeshBooleanTask FINISHED"));
}
