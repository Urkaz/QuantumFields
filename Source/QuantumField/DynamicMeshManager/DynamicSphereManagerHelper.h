// Copyright Catness Game Studios. All Rights Reserved.

#pragma once

#include "UDynamicMesh.h"
#include "Data/QF_BooleanModifier.h"
#include "Subsystems/EngineSubsystem.h"
#include "Tasks/ApplySphereBooleanTask.h"
#include "DynamicSphereManagerHelper.generated.h"

class UDynamicMeshComponent;

UCLASS()
class QUANTUMFIELD_API UDynamicSphereManagerHelper : public UObject
{
	GENERATED_BODY()

public:
	static bool IsMeshInsideSpheres(const UMeshComponent* Mesh);
	static bool IsMeshOutsideAllSpheres(const UMeshComponent* Mesh);
};
