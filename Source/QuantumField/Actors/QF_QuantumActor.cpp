// Fill out your copyright notice in the Description page of Project Settings.


#include "QF_QuantumActor.h"
#include "Kismet/GameplayStatics.h"
#include "QuantumField/QuantumFieldGameMode.h"
#include "QuantumField/Interfaces/QF_IConstructionScript.h"
#include "QuantumField/Interfaces/QF_IQuantumComponent.h"

// Sets default values
AQF_QuantumActor::AQF_QuantumActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bBelongsPast = false;
}

// Called when the game starts or when spawned
void AQF_QuantumActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> ComponentsFound;
	GetComponents(ComponentsFound);

	for (UActorComponent* Component : ComponentsFound)
	{
		if (Component->GetClass()->ImplementsInterface(UQF_IQuantumComponent::StaticClass()))
		{
			bool bWorldStateIsPast = false;
			if (const AQuantumFieldGameMode* GameMode = Cast<AQuantumFieldGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				bWorldStateIsPast = GameMode->bWorldStateIsPast;
			}
		
			IQF_IQuantumComponent::Execute_SetInitialState(Component, bBelongsPast);
			IQF_IQuantumComponent::Execute_ChangeState(Component, bBelongsPast, bWorldStateIsPast, nullptr);
		}
	}
}

void AQF_QuantumActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	TArray<UActorComponent*> ComponentsFound;
	GetComponents(ComponentsFound);

	for (UActorComponent* Component : ComponentsFound)
	{
		if (Component->GetClass()->ImplementsInterface(UQF_IConstructionScript::StaticClass()))
		{
			IQF_IConstructionScript::Execute_CodeConstructionScript(Component);
		}
	}
#endif
}

void AQF_QuantumActor::ChangeState_Implementation(const bool bIsPast, AQuantumSphere* Sphere)
{
	TArray<UActorComponent*> ComponentsFound;
	GetComponents(ComponentsFound);

	for (UActorComponent* Component : ComponentsFound)
	{
		if (Component->GetClass()->ImplementsInterface(UQF_IQuantumComponent::StaticClass()))
		{
			IQF_IQuantumComponent::Execute_ChangeState(Component, bBelongsPast, bIsPast, Sphere);
		}
	}
}
