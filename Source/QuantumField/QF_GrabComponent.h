// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "QF_GrabComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Physics, Common), hidecategories=(Object,Activation,"Components|Activation"), ShowCategories=(Mobility), editinlinenew, meta=(BlueprintSpawnableComponent))
class QUANTUMFIELD_API UQF_GrabComponent : public UPhysicsHandleComponent
{
	GENERATED_BODY()

public:
	UQF_GrabComponent();
	
	UFUNCTION(BlueprintCallable, Category = "QF_PhysicsHandle")
	void Grab();

	UFUNCTION(BlueprintCallable, Category = "QF_PhysicsHandle")
	void Release();

	bool IsPicking();
	
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	FName PickupTagName;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float PickupRange;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float MaxGrabThreshold;
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	float ObjectDistanceToFace;
	
protected:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bIsPicking;
	AActor* PickedActor;
};
