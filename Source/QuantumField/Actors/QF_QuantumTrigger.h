// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QF_QuantumActor.h"
#include "QF_QuantumTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTriggerStateChanged, bool, NewSatate, AQF_QuantumTrigger*, Trigger);

UCLASS(Abstract)
class QUANTUMFIELD_API AQF_QuantumTrigger : public AQF_QuantumActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQF_QuantumTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetTriggerState();

	UFUNCTION(BlueprintCallable)
	virtual void SetTriggerState(bool bNewState);

	UFUNCTION(BlueprintCallable, CallInEditor)
	virtual void ToggleTriggerState();

	UPROPERTY(BlueprintAssignable)
	FOnTriggerStateChanged OnTriggerStateChanged;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere,  meta = (AllowPrivateAccess))
	bool bTriggerState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="QuantumTrigger",  meta = (AllowPrivateAccess))
	bool bActivateOnlyOnce;

protected:
	
	bool bActivationComplete;

};
