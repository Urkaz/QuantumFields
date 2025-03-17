// Fill out your copyright notice in the Description page of Project Settings.


#include "QF_QuantumTriggerable.h"
#include "QF_QuantumTrigger.h"

// Sets default values
AQF_QuantumTriggerable::AQF_QuantumTriggerable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bTriggereableState = false;
	bActivateOnlyOnce = false;
	bActivationComplete = false;
}

// Called when the game starts or when spawned
void AQF_QuantumTriggerable::BeginPlay()
{
	Super::BeginPlay();

	BindToTriggers();
}

// Called every frame
void AQF_QuantumTriggerable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQF_QuantumTriggerable::OnTriggereableOpened_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Triggereable Opened"));
}

void AQF_QuantumTriggerable::OnTriggereableClossed_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Triggereable Clossed"));
}

void AQF_QuantumTriggerable::RegisterTriggerState(bool bNewState, AQF_QuantumTrigger* Trigger)
{
	if (Trigger != nullptr)
	{
		const int TriggerIndex = Triggers.Find(Trigger);
		
		if (TriggerIndex != INDEX_NONE)
		{
			bTriggers[TriggerIndex] = bNewState;
			CheckTriggereableState();
		}
	}
}

void AQF_QuantumTriggerable::BindToTriggers()
{
	bTriggers.Empty();
	for (AQF_QuantumTrigger* Element : Triggers)
	{
		if (Element)
		{
			Element->OnTriggerStateChanged.AddDynamic(this, &AQF_QuantumTriggerable::RegisterTriggerState);
			bTriggers.Add(Element->GetTriggerState());
		}
		else
		{
			bTriggers.Add(true);
		}
	}
}

bool AQF_QuantumTriggerable::CheckTriggerState()
{
	bool bTriggersEnabled = true;
	for (const bool Element : bTriggers)
	{
		bTriggersEnabled = bTriggersEnabled && Element;
	}

	return bTriggersEnabled;
}

void AQF_QuantumTriggerable::CheckTriggereableState()
{
	if (!bActivationComplete)
	{
		const bool NewState = CheckTriggerState();
		if (NewState != bTriggereableState)
		{
			if (NewState)
			{
				OnTriggereableOpened();
			}
			else
			{
				OnTriggereableClossed();
			}
			bTriggereableState = NewState;
			
			OnTriggereableStateChange.Broadcast(NewState);
			if (bActivateOnlyOnce)
			{
				bActivationComplete = true;
			}
		}
	}
}

