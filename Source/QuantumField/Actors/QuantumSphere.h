// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"
#include "QuantumField/DynamicMeshManager/Data/QF_BooleanModifier.h"
#include "QuantumSphere.generated.h"

class AQF_QuantumActor;
class FNotifyEndTask;
class USphereComponent;

//SphereIndex will return the index of the Sphere if we have more than 1 Sphere
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSphereFinishedAction, int32, SphereIndex, class AQuantumSphere*, Ptr);

UCLASS()
class AQuantumSphere : public AActor
{
	GENERATED_BODY()

	friend FNotifyEndTask;

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	UDynamicMeshComponent* SphereMeshComponent;
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USphereComponent* SphereCollisionComponent;

	UPROPERTY(VisibleAnywhere)
	int32 SphereIndex{0};
	bool Interrupted{false};

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SphereSetup)
	float SphereRadius{250};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SphereSetup)
	int32 SphereMeshSteps{10};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SphereSetup)
	UMaterialInterface* SphereMaterial;
	UPROPERTY(EditAnywhere, Category=SphereSetup)
	bool StaticSphere{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SphereSetup)
	UMaterialParameterCollection* MaterialParameterCollection;

	UPROPERTY(BlueprintAssignable)
	FSphereFinishedAction OnSphereFinishedActivating;
	UPROPERTY(BlueprintAssignable)
	FSphereFinishedAction OnSphereFailedActivating;
	UPROPERTY(BlueprintAssignable)
	FSphereFinishedAction OnSphereFinishedDeactivating;

protected:
	float CurrentRadius{0};
	float FinalRadius{0};

	bool NotifyEndActivateEnabled{false};
	bool NotifyEndDeactivateEnabled{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic* SphereMaterialInstance;

	FQF_BooleanModifier BooleanModifier;

public:
	AQuantumSphere();

	UPROPERTY()
	TArray<AQF_QuantumActor*> QuantumActors;

	UFUNCTION(BlueprintCallable)
	int32 GetSphereIndex() const;

	const FQF_BooleanModifier& GetBooleanModifier() const;

	/*
	 * ACTIVATE
	 */
public:
	UFUNCTION(BlueprintCallable)
	void SphereActivate(int32 Index);
	UFUNCTION(BlueprintCallable)
	void TryNotifyEndActivate();
	void NotifyEndActivate();
	void BeginSphereActivate(UDynamicMesh* DynamicMesh);
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_SphereActivate();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_SphereFinishedActivating();
	UFUNCTION(BlueprintCallable)
	void Update_Activating(float ScaleMultiplier, bool CheckOverlaps);

	/*
	 * DEACTIVATE
	 */
public:
	UFUNCTION(BlueprintCallable)
	void SphereDeactivate();
	UFUNCTION(BlueprintCallable)
	void TryNotifyEndDeactivate();
	void NotifyEndDeactivate();
	UFUNCTION(BlueprintCallable)
	void NotifyDestroy();
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_SphereDeactivate();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_SphereFinishedDeactivating();
	UFUNCTION(BlueprintCallable)
	void Update_Deactivating(float ScaleMultiplier);

	/*
	 * INTERRUPT
	 */
public:
	UFUNCTION(BlueprintCallable)
	void InterruptActivate();
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_SphereInterruptActivate();


protected:
	virtual void BeginPlay() override;

	void UpdateSphereRadius(float ScaleMultiplier);

	void UpdateMaterialPosition(FVector Position) const;
	void UpdateMaterialRadius(float Radius) const;
	void UpdateMaterialSphereIndex(int InSphereIndex) const;
};
