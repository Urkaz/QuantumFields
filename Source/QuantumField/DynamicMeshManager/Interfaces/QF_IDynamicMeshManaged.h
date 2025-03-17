// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QF_IDynamicMeshManaged.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQF_IDynamicMeshManaged : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class QUANTUMFIELD_API IQF_IDynamicMeshManaged
{
	GENERATED_BODY()
public:
	virtual void SetupPrerequisites(bool bNewStateIsPast, const FGraphEventRef& ParentTask) {};
	//virtual void NotifyEnd(bool bNewStateIsPast) = 0;
};
