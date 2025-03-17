// Fill out your copyright notice in the Description page of Project Settings.

#include "QuantumFieldEditor/Public/QuantumActorActionUtility.h"


void UQuantumActorActionUtility::ManageTimeline(AActor* Actor, bool bIsPast)
{
	if (!Actor) return;

	TArray<UStaticMeshComponent*> Components;
	Actor->GetComponents(UStaticMeshComponent::StaticClass(), Components);
	for (UStaticMeshComponent* Component : Components)
	{
		TArray<UMaterialInterface*> Materials = Component->GetMaterials();
		for(UMaterialInterface* Material : Materials)
		{
			UMaterialInstance* MaterialInstance = Cast<UMaterialInstance>(Material);
			for(FScalarParameterValue ScalarParameter : MaterialInstance->ScalarParameterValues)
			{
				if (ScalarParameter.ParameterInfo.Name == "IsPast")
				{
					if (bIsPast)
					{
						ScalarParameter.ParameterValue = 1;
					}
					else
					{
						ScalarParameter.ParameterValue = 0;
					}
					MaterialInstance->ForceRecompileForRendering();
				}
			}
		}
	}
}
