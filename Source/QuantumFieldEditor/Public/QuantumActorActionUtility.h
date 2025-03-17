// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorActionUtility.h"
#include "EditorUtilityObject.h"
#include "IEditorUtilityExtension.h"
#include "QuantumActorActionUtility.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class QUANTUMFIELDEDITOR_API UQuantumActorActionUtility : public UEditorUtilityObject, public IEditorUtilityExtension
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "QuantumActorActionUtility")
	virtual void ManageTimeline(AActor* Actor, bool bIsPast);
	
};
