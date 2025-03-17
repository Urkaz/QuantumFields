// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshManager/DynamicSphereManager.h"
#include "GameFramework/GameModeBase.h"
#include "QuantumFieldGameMode.generated.h"

class AQuantumSphere;
UCLASS(minimalapi)
class AQuantumFieldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AQuantumFieldGameMode();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWorldStateIsPast {false};
	UPROPERTY(Transient)
	UDynamicSphereManager* DynamicSphereManager;
};



