// Copyright Catness Game Studios. All Rights Reserved.

#pragma once

#include "DynamicMeshActor.h"
#include "UDynamicMesh.h"
#include "Components/QF_QuantumMeshComponent.h"
#include "Data/QF_BooleanModifier.h"
#include "DynamicSphereManager.generated.h"

class AQuantumSphere;

UCLASS()
class QUANTUMFIELD_API UDynamicSphereManager : public UObject
{
	GENERATED_BODY()

public:
	FGraphEventRef RequestCopyMesh(UQF_QuantumMeshComponent* Component, UStaticMesh* MeshToCopy, UDynamicMesh* TargetMesh, const FGraphEventRef& ParentTask);
	FGraphEventRef RequestBoolean(UQF_QuantumMeshComponent* Component, bool bSubtract, UDynamicMesh* TargetMesh, const FGraphEventRef& ParentTask);

	void RequestNotifyFinish(UQF_QuantumMeshComponent* Component, const FGraphEventRef& ParentTask);

protected:
	FGraphEventArray RequestNotifyFinishPrerequisites;

public:
	/** Control whether the DynamicMeshPool will be created when requested via GetComputeMeshPool() */
	UPROPERTY(Category = "DynamicMesh|Advanced", EditAnywhere, BlueprintReadWrite)
	bool bEnableComputeMeshPool = false;
protected:
	/** The internal Mesh Pool, for use in DynamicMeshActor BPs. Use GetComputeMeshPool() to access this, as it will only be created on-demand if bEnableComputeMeshPool = true */
	UPROPERTY(Transient)
	TObjectPtr<UDynamicMeshPool> DynamicMeshPool;
	UPROPERTY(Transient)
	UDynamicMesh* CurrentSphereMesh{nullptr};
	TArray<FQF_BooleanModifier> SphereModifiers;

	int32 MeshesInPool{0};
	int32 MeshesInUse{0};

public:
	UDynamicMeshPool* GetComputeMeshPool();
	UDynamicMesh* AllocateComputeMesh();
	bool ReleaseComputeMesh(UDynamicMesh* Mesh);

	UDynamicMesh* GetSpheresBoolean() const;
	void SetSpheresBoolean(UDynamicMesh* DynamicMesh);
	const TArray<FQF_BooleanModifier>& GetSpheresModifiers() const;

	AQuantumSphere* SpawnSphere(UWorld* World, TSubclassOf<AQuantumSphere> SphereClass, FVector Location);
	void DespawnSphere(UWorld* World, AQuantumSphere* Sphere);

protected:
	void AddNewSphere(const FQF_BooleanModifier& Modifier);
	void DeleteSphere(const FQF_BooleanModifier& Modifier);
	TGraphTask<FApplySphereBooleanTask>* CreateBooleanSphereTask(TArray<FQF_BooleanModifier> TaskModifiers);

	FGraphEventRef NotifyTask;

public:
	static UDynamicSphereManager* Get(const UObject* WorldContextObject);
};
