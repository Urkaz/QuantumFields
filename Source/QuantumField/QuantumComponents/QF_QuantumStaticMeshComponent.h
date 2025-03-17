// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "QuantumField/Interfaces/QF_IQuantumComponent.h"
#include "QF_QuantumStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Rendering, Common), hidecategories=(Object,Activation,"Components|Activation"), ShowCategories=(Mobility), editinlinenew, meta=(BlueprintSpawnableComponent))
class QUANTUMFIELD_API UQF_QuantumStaticMeshComponent : public UStaticMeshComponent, public IQF_IQuantumComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void ChangeState(bool bOwnerIsPast, bool bNewStateIsPast, AQuantumSphere* Sphere);
	virtual void ChangeState_Implementation(bool bOwnerIsPast, bool bNewStateIsPast, AQuantumSphere* Sphere) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void SetInitialState(bool bIsPast);
	virtual void SetInitialState_Implementation(bool bIsPast) override;

};
