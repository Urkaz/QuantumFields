// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuantumFieldGameMode.h"

#include "UObject/ConstructorHelpers.h"

AQuantumFieldGameMode::AQuantumFieldGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AQuantumFieldGameMode::BeginPlay()
{
	Super::BeginPlay();
}
