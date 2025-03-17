// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QF_IConstructionScript.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQF_IConstructionScript : public UInterface
{
	GENERATED_BODY()
};

class AQuantumSphere;

/**
 * 
 */
class QUANTUMFIELD_API IQF_IConstructionScript
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "QuantumComponents")
	void CodeConstructionScript();
};
