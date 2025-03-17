#include "ApplySphereBooleanTask.h"

#include "GeometryScript/MeshBooleanFunctions.h"
#include "QuantumField/DynamicMeshManager/DynamicSphereManager.h"

void FApplySphereBooleanTask::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	FGeometryScriptMeshBooleanOptions BooleanOptions;
	BooleanOptions.bFillHoles = true;
	BooleanOptions.bSimplifyOutput = true;
	for (const FQF_BooleanModifier& Modifier : Modifiers)
	{
		DynamicMesh = UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshBoolean(DynamicMesh,
																						  FTransform(),
																						  Modifier.Mesh,
																						  Modifier.Transform,
																						  EGeometryScriptBooleanOperation::Union,
																						  BooleanOptions);
	}

	ManagerPtr->SetSpheresBoolean(DynamicMesh);

	//UE_LOG(LogTemp, Warning, TEXT("FApplySphereBooleanTask FINISHED"));
}