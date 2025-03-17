// Fill out your copyright notice in the Description page of Project Settings.


#include "QF_QuantumStaticMeshComponent.h"

#include "ProceduralMeshComponent.h"

void UQF_QuantumStaticMeshComponent::ChangeState_Implementation(bool bOwnerIsPast, bool bNewStateIsPast, AQuantumSphere* Sphere)
{
	//As an example, it can be modified and expanded.
	if (bOwnerIsPast == bNewStateIsPast)
	{
		SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Block);
	}
	else
	{
		SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Ignore);
	}
}

void UQF_QuantumStaticMeshComponent::SetInitialState_Implementation(const bool bIsPast)
{
	const TArray<UMaterialInterface*> Materials = GetMaterials();
	for (int i = 0; i < Materials.Num(); ++i)
	{
		if (UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(Materials[i], this))
		{
			MaterialInstanceDynamic->SetScalarParameterValue(TEXT("IsPast"), bIsPast ? 1 : 0);

			SetMaterial(i, MaterialInstanceDynamic);
		}
	}
	SetRenderCustomDepth(true);
	SetCustomDepthStencilValue(1);
}
