// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QF_QuantumActor.generated.h"

class AQuantumSphere;
UCLASS()
class QUANTUMFIELD_API AQF_QuantumActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQF_QuantumActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumAcotr")
	void ChangeState(bool bIsPast, AQuantumSphere* Sphere);

	//Set if belongs to the past, else belongs to the future
	UPROPERTY(EditAnywhere, Category="QuantumProperties")
	bool bBelongsPast;
};
