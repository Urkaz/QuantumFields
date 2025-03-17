// Fill out your copyright notice in the Description page of Project Settings.


#include "QF_QuantumMeshComponent.h"

#include "GeometryScript/MeshAssetFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "Kismet/GameplayStatics.h"
#include "QuantumField/Actors/QuantumSphere.h"
#include "QuantumField/DynamicMeshManager/DynamicSphereManager.h"
#include "QuantumField/DynamicMeshManager/DynamicSphereManagerHelper.h"

#define ECC_Projectile ECC_GameTraceChannel1
#define ECC_Pickup ECC_GameTraceChannel2
#define ECC_ForceField ECC_GameTraceChannel3
#define ECC_Sphere ECC_GameTraceChannel4

class FCopyStaticMeshTask;

void UQF_QuantumMeshComponent::ChangeState_Implementation(bool bOwnerIsPast, bool bNewStateIsPast, AQuantumSphere* Sphere)
{
	const bool StateEnabled = bOwnerIsPastState == bNewStateIsPast;
	bool InsideOrOutside = UDynamicSphereManagerHelper::IsMeshInsideSpheres(StaticMeshComponent) || UDynamicSphereManagerHelper::IsMeshOutsideAllSpheres(StaticMeshComponent);
	UE_LOG(LogTemp, Warning, TEXT("[%s] InsideOrOutside: %s // StateEnabled: %s"), *GetReadableName(), InsideOrOutside ? TEXT("true") : TEXT("false"), StateEnabled ? TEXT("true") : TEXT("false"));
	if (InsideOrOutside)
	{
		SetPreviewMeshVisible(true);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, StateEnabled ? ECR_Block : ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Camera, StateEnabled ? ECR_Block : ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Projectile, StateEnabled ? ECR_Block : ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pickup, StateEnabled ? ECR_Block : ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, StateEnabled ? ECR_Block : ECR_Ignore);

		SetHiddenInGame(true);
		SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
		SetCollisionResponseToChannel(ECC_Pickup, ECR_Ignore);
		SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}
	else
	{
		//Hide the static mesh
		SetPreviewMeshVisible(false);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pickup, ECR_Ignore);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);

		//Apply the DynamicMesh changes and make it visible
		if(!TempMesh)
			UE_LOG(LogTemp, Error, TEXT("[%s] TempMesh IS NULL"), *GetReadableName());
		SetDynamicMesh(TempMesh);
		TempMesh = nullptr;

		UpdateCollision();
		SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
		SetCollisionResponseToChannel(ECC_Projectile, ECR_Block);
		SetCollisionResponseToChannel(ECC_Pickup, ECR_Block);
		SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);

		SetHiddenInGame(false);
	}
}

void UQF_QuantumMeshComponent::SetInitialState_Implementation(const bool bOwnerIsPast)
{
	bOwnerIsPastState = bOwnerIsPast;

	//Initialize Materials
	TArray<UMaterialInterface*> MeshMaterials = GetMaterialsFromMesh();
	for (int i = 0; i < MeshMaterials.Num(); ++i)
	{
		if (UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(MeshMaterials[i], this))
		{
			MaterialInstanceDynamic->SetScalarParameterValue(TEXT("IsPast"), bOwnerIsPast ? 1 : 0);
			MaterialInstanceDynamic->SetScalarParameterValue(TEXT("DisableQuantumOpacity"), 1);
			SetMaterial(i, MaterialInstanceDynamic);
		}
		if (UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(MeshMaterials[i], this))
		{
			MaterialInstanceDynamic->SetScalarParameterValue(TEXT("IsPast"), bOwnerIsPast ? 1 : 0);
			MaterialInstanceDynamic->SetScalarParameterValue(TEXT("DisableQuantumOpacity"), 0);
			StaticMeshComponent->SetMaterial(i, MaterialInstanceDynamic);
		}
	}

	EnableComplexAsSimpleCollision();
	bDeferCollisionUpdates = true;
	SetCollisionProfileName(TEXT("BlockAll"));
	SetCollisionObjectType(ECC_WorldDynamic);
	SetEnableGravity(false);
}

void UQF_QuantumMeshComponent::SetupPrerequisites(bool bNewStateIsPast, const FGraphEventRef& ParentTask)
{
	//We always display the static mesh, and depending on the output, it may be hidden again
	SetPreviewMeshVisible(true);

	bool InsideOrOutside = UDynamicSphereManagerHelper::IsMeshInsideSpheres(StaticMeshComponent) || UDynamicSphereManagerHelper::IsMeshOutsideAllSpheres(StaticMeshComponent);
	if (!InsideOrOutside)
	{
		UDynamicSphereManager* DynMeshMgr = UDynamicSphereManager::Get(GetWorld());
		TempMesh = DynMeshMgr->AllocateComputeMesh();
		const FGraphEventRef& CopyTask = UDynamicSphereManager::Get(GetWorld())->RequestCopyMesh(this, Mesh, TempMesh, ParentTask);
		const FGraphEventRef& BooleanTask = UDynamicSphereManager::Get(GetWorld())->RequestBoolean(this, !bOwnerIsPastState, TempMesh, CopyTask);
		UDynamicSphereManager::Get(GetWorld())->RequestNotifyFinish(this, BooleanTask);
	}
}

void UQF_QuantumMeshComponent::CodeConstructionScript_Implementation()
{
	if (!StaticMeshComponent)
	{
		StaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		StaticMeshComponent->RegisterComponent();
		StaticMeshComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
	StaticMeshComponent->SetWorldTransform(GetComponentTransform());
	StaticMeshComponent->SetStaticMesh(Mesh);
}

TArray<UMaterialInterface*> UQF_QuantumMeshComponent::GetMaterialsFromMesh() const
{
	TArray<UMaterialInterface*> OutMaterials;
	const int32 TotalNumMaterials = Mesh->GetStaticMaterials().Num();;
	if (TotalNumMaterials > 0)
	{
		// make sure to extend it
		OutMaterials.AddZeroed(TotalNumMaterials);

		for (int32 MaterialIndex = 0; MaterialIndex < TotalNumMaterials; ++MaterialIndex)
		{
			OutMaterials[MaterialIndex] = Mesh->GetMaterial(MaterialIndex);
		}
	}

	return OutMaterials;
}

void UQF_QuantumMeshComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UQF_QuantumMeshComponent::SetPreviewMeshVisible(const bool Visible) const
{
	StaticMeshComponent->SetHiddenInGame(!Visible);
}
