// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QF_IQuantumComponent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQF_IQuantumComponent : public UInterface
{
	GENERATED_BODY()
};

class AQuantumSphere;

/**
 * 
 */
class QUANTUMFIELD_API IQF_IQuantumComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void ChangeState(bool bOwnerIsPast, bool bNewStateIsPast, AQuantumSphere* Sphere);

	//Sets the initial state of the component. Similar to BeginPlay, but with an bOwnerIsPast as input
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void SetInitialState(bool bOwnerIsPast);
};
