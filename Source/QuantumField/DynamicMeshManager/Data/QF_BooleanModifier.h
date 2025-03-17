// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UDynamicMesh.h"
#include "CoreMinimal.h"
#include "QF_BooleanModifier.generated.h"

USTRUCT()
struct QUANTUMFIELD_API FQF_BooleanModifier
{
	GENERATED_BODY()

	FQF_BooleanModifier() : Mesh(nullptr)
	{
	}

	FQF_BooleanModifier(UDynamicMesh* InMesh, const FTransform InTransform, float InSphereadius)
		: Mesh(InMesh), Transform(InTransform), SphereRadius(InSphereadius)
	{
	}

	UPROPERTY()
	UDynamicMesh* Mesh;
	FTransform Transform;
	float SphereRadius{250};

	bool operator==(const FQF_BooleanModifier& Other) const
	{
		return Mesh == Other.Mesh && Transform.Equals(Other.Transform);
	}
};
