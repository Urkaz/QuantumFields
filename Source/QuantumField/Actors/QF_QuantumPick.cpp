// Fill out your copyright notice in the Description page of Project Settings.


#include "QF_QuantumPick.h"
#include "NiagaraFunctionLibrary.h"
#include "QuantumField/QF_GrabComponent.h"


AQF_QuantumPick::AQF_QuantumPick()
{
	PickupTagName="Pickable";
	Tags.Add(PickupTagName);

	NumSpheres = 0;
}

void AQF_QuantumPick::BeginPlay()
{
	Super::BeginPlay();

	tInitialPosition = GetActorTransform();

	SetInitialStatus();
}

void AQF_QuantumPick::TryReset_Implementation()
{
	NumSpheres -= 1;

	if(NumSpheres == 0)
	{
		if(ensure(ResetParticles))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ResetParticles, GetActorLocation(), GetActorRotation(), GetActorScale());
		}

		//Free the object
		GoFree();
		
		SetActorTransform(tInitialPosition);
		SetInitialStatus();
	}
}

void AQF_QuantumPick::ExitSphere_Implementation()
{

}

void AQF_QuantumPick::EnterSphere_Implementation()
{
	NumSpheres += 1;
}

void AQF_QuantumPick::Picked_Implementation(bool bIsPicked, UActorComponent* GrabComponent)
{
	GrabComp = GrabComponent;
	SetEffect(bIsPicked);
}

bool AQF_QuantumPick::GoFree_Implementation()
{
	if(GrabComp)
	{
		Cast<UQF_GrabComponent>(GrabComp)->Release();
		return true;
	}

	return false;
}

void AQF_QuantumPick::SetEffect_Implementation(bool bIsPicked)
{
}

void AQF_QuantumPick::SetInitialStatus_Implementation()
{
}
