// Fill out your copyright notice in the Description page of Project Settings.


#include "QF_QuantumTrigger.h"

// Sets default values
AQF_QuantumTrigger::AQF_QuantumTrigger():
	bTriggerState(false),
	bActivateOnlyOnce(false),
	bActivationComplete(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQF_QuantumTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQF_QuantumTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AQF_QuantumTrigger::GetTriggerState()
{
	return bTriggerState;
}

void AQF_QuantumTrigger::ToggleTriggerState()
{
	SetTriggerState(!bTriggerState);
}

void AQF_QuantumTrigger::SetTriggerState(bool bNewState)
{
	if (!bActivationComplete)
	{
		bTriggerState = bNewState;
		OnTriggerStateChanged.Broadcast(bTriggerState, this);
		
		if (bActivateOnlyOnce)
		{
			bActivationComplete = true;
		}
	}
}
