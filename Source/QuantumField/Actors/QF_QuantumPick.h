// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "QF_QuantumActor.h"
#include "QuantumField/Interfaces/QF_IQuantumPickOject.h"
#include "QF_QuantumPick.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMFIELD_API AQF_QuantumPick : public AQF_QuantumActor, public  IQF_IQuantumPickOject
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	AQF_QuantumPick();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void TryReset();
	virtual void TryReset_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void ExitSphere();
	virtual void ExitSphere_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void EnterSphere();
	virtual void EnterSphere_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void Picked(bool bIsPicked, UActorComponent* GrabComponent);
	virtual void Picked_Implementation(bool bIsPicked, UActorComponent* GrabComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	bool GoFree();
	virtual bool GoFree_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void SetInitialStatus();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void SetEffect(bool bIsPicked);
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	UNiagaraSystem* ResetParticles;
	
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	FName PickupTagName;

private:
	FTransform tInitialPosition;
	UActorComponent* GrabComp;
	int32 NumSpheres;
};
