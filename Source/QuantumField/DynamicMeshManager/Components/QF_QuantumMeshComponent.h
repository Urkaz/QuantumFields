// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"
#include "QuantumField/DynamicMeshManager/Interfaces/QF_IDynamicMeshManaged.h"
#include "QuantumField/Interfaces/QF_IConstructionScript.h"
#include "QuantumField/Interfaces/QF_IQuantumComponent.h"
#include "QF_QuantumMeshComponent.generated.h"

class FApplySphereBooleanTask;

UCLASS(Blueprintable, ClassGroup=(Rendering, Common), hidecategories=(Object,Activation,"Components|Activation"), ShowCategories=(Mobility), editinlinenew, meta=(BlueprintSpawnableComponent))
class QUANTUMFIELD_API UQF_QuantumMeshComponent : public UDynamicMeshComponent, public IQF_IQuantumComponent, public IQF_IConstructionScript, public IQF_IDynamicMeshManaged
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "QuantumMeshComponent")
	UStaticMesh* Mesh;

	bool bOwnerIsPastState{false};

	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

public:
	/* Interface IQF_IQuantumComponent Start */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumMeshComponent")
	void ChangeState(bool bOwnerIsPast, bool bNewStateIsPast, AQuantumSphere* Sphere);
	virtual void ChangeState_Implementation(bool bOwnerIsPast, bool bNewStateIsPast, AQuantumSphere* Sphere) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumMeshComponent")
	void SetInitialState(bool bOwnerIsPast);
	virtual void SetInitialState_Implementation(bool bOwnerIsPast) override;
	/* Interface IQF_IQuantumComponent End */

	/* Interface IQF_IDynamicMeshManaged Start */
	virtual void SetupPrerequisites(bool bNewStateIsPast, const FGraphEventRef& ParentTask) override;
	//virtual void StoreTemporalDynamicMesh(UDynamicMesh* TemporalMesh) override;
	/* Interface IQF_IDynamicMeshManaged End */

	/* Interface IQF_IConstructionScript Start */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumMeshComponent")
	void CodeConstructionScript();
	virtual void CodeConstructionScript_Implementation() override;
	/* Interface IQF_IConstructionScript End */

protected:
	TArray<UMaterialInterface*> GetMaterialsFromMesh() const;

	virtual void BeginPlay() override;

	UPROPERTY(Transient)
	UDynamicMesh* TempMesh;

	void SetPreviewMeshVisible(bool Visible) const;
};
