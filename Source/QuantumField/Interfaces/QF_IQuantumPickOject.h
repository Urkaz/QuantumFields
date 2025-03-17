// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QF_IQuantumPickOject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQF_IQuantumPickOject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class QUANTUMFIELD_API IQF_IQuantumPickOject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumPickupInterface")
	void TryReset();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumPickupInterface")
	void ExitSphere();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumPickupInterface")
	void EnterSphere();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumPickupInterface")
	void Picked(bool bIsPicked, UActorComponent* GrabComponent);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumPickupInterface")
	bool GoFree();
	
};
