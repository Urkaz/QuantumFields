// Fill out your copyright notice in the Description page of Project Settings.


#include "QF_GrabComponent.h"

#include "Interfaces/QF_IQuantumPickOject.h"
#include "Kismet/KismetMathLibrary.h"

UQF_GrabComponent::UQF_GrabComponent()
{
	SetComponentTickEnabled(false);
	PickupTagName = "Pickable";
	PickupRange = 500;
	ObjectDistanceToFace = 200;

	MaxGrabThreshold = ObjectDistanceToFace + 20;
	
	bIsPicking =false;
}

void UQF_GrabComponent::Grab()
{
	if(bIsPicking == false)
	{
		FVector LocationEyes;
		FRotator RotationEyes;
		FHitResult Hit;
		GetOwner()->GetActorEyesViewPoint(LocationEyes,RotationEyes);
		if(GetWorld()->LineTraceSingleByChannel(Hit, LocationEyes,LocationEyes+(UKismetMathLibrary::GetForwardVector(RotationEyes)*PickupRange),ECollisionChannel::ECC_EngineTraceChannel1) && Hit.GetActor()->Tags.Contains(PickupTagName))
		{
			PickedActor = Hit.GetActor();
			if (PickedActor->GetClass()->ImplementsInterface(UQF_IQuantumPickOject::StaticClass()))
			{
				IQF_IQuantumPickOject::Execute_Picked(PickedActor, true, this);
			}
			
			GrabComponentAtLocation(Hit.Component.Get(),FName(),Hit.Component->GetComponentLocation());
			SetComponentTickEnabled(true);
		}
	}
}

void UQF_GrabComponent::Release()
{
	if (!PickedActor)
	{
		return;
	}

	if (PickedActor->GetClass()->ImplementsInterface(UQF_IQuantumPickOject::StaticClass()))
	{
		IQF_IQuantumPickOject::Execute_Picked(PickedActor, false, nullptr);
	}
	
	PickedActor = nullptr;
	
	ReleaseComponent();
	bIsPicking = false;
	SetComponentTickEnabled(false);
}

bool UQF_GrabComponent::IsPicking()
{
	return bIsPicking;
}

void UQF_GrabComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector LocationEyes;
	FRotator RotationEyes;
	GetOwner()->GetActorEyesViewPoint(LocationEyes,RotationEyes);
	SetTargetLocation(LocationEyes+(UKismetMathLibrary::GetForwardVector(RotationEyes)*ObjectDistanceToFace));

	if(PickedActor && FVector::Distance(PickedActor->GetActorLocation(), GetOwner()->GetActorLocation()) > MaxGrabThreshold)
	{
		Release();
	}
}
