// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QF_QuantumActor.h"
#include "QF_QuantumTriggerable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggereableStateChange, bool, NewSatate);

class AQF_QuantumTrigger;

UCLASS(Abstract)
class QUANTUMFIELD_API AQF_QuantumTriggerable : public AQF_QuantumActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQF_QuantumTriggerable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnTriggereableOpened();

	UFUNCTION(BlueprintNativeEvent)
	void OnTriggereableClossed();

	UFUNCTION()
	void RegisterTriggerState(bool bNewState, AQF_QuantumTrigger* Trigger);

	UPROPERTY(BlueprintAssignable)
	FOnTriggereableStateChange OnTriggereableStateChange;
	
protected:
	void BindToTriggers();

	bool CheckTriggerState();

	void CheckTriggereableState();

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess))
	TArray<AQF_QuantumTrigger*> Triggers;

	TArray<bool> bTriggers;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool bTriggereableState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="QuantumTriggerable",  meta = (AllowPrivateAccess))
	bool bActivateOnlyOnce;

protected:
	
	bool bActivationComplete;
};
