#include "CopyStaticMeshTask.h"

#include "GeometryScript/MeshAssetFunctions.h"

void FCopyStaticMeshTask::DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	FGeometryScriptCopyMeshFromAssetOptions CopyOptions;
	CopyOptions.bRequestTangents = false;
	CopyOptions.bApplyBuildSettings = false;
	FGeometryScriptMeshReadLOD LODOptions;
	LODOptions.LODType = EGeometryScriptLODType::RenderData;
	EGeometryScriptOutcomePins Enum;
	UGeometryScriptLibrary_StaticMeshFunctions::CopyMeshFromStaticMesh(Mesh, TargetMesh, CopyOptions, LODOptions, Enum, nullptr);

	//UE_LOG(LogTemp, Warning, TEXT("FCopyStaticMeshTask FINISHED"));
}
