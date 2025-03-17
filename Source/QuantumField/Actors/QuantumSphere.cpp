#include "QuantumSphere.h"

#include "QF_QuantumActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "QuantumField/QuantumFieldCharacter.h"
#include "QuantumField/QuantumFieldGameMode.h"
#include "QuantumField/DynamicMeshManager/DynamicSphereManager.h"
#include "QuantumField/Interfaces/QF_IQuantumPickOject.h"

#define ECC_Pickup ECC_GameTraceChannel2
#define ECC_Sphere ECC_GameTraceChannel4

AQuantumSphere::AQuantumSphere()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SphereMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("SphereMesh"));
	SphereMeshComponent->SetupAttachment(SceneComponent);
	SphereMeshComponent->SetRelativeScale3D(FVector(0));

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollisionComponent->SetupAttachment(SceneComponent);
	SphereCollisionComponent->SetSphereRadius(0);
}

void AQuantumSphere::BeginPlay()
{
	Super::BeginPlay();

	if (SphereMaterial)
	{
		SphereMaterialInstance = UMaterialInstanceDynamic::Create(SphereMaterial, this);
		SphereMeshComponent->SetMaterial(0, SphereMaterialInstance);
	}

	FinalRadius = SphereRadius;
}

/*
 * ACTIVATE
 */

void AQuantumSphere::BeginSphereActivate(UDynamicMesh* DynamicMesh)
{
	SphereMeshComponent->SetDynamicMesh(DynamicMesh);
	BooleanModifier = FQF_BooleanModifier(DynamicMesh, GetActorTransform(), SphereRadius);

	SphereMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereMeshComponent->SetCollisionObjectType(ECC_Sphere);
	SphereMeshComponent->SetCollisionResponseToChannel(ECC_Pickup, ECR_Ignore);
	SphereMeshComponent->EnableComplexAsSimpleCollision();
}

void AQuantumSphere::SphereActivate(const int32 Index)
{
	SphereIndex = Index;

	UpdateMaterialPosition(GetActorLocation());
	UpdateMaterialSphereIndex(SphereIndex);

	if (StaticSphere)
	{
		UpdateSphereRadius(1);
		NotifyEndActivate();
	}
	else
	{
		BP_SphereActivate();
	}
}

void AQuantumSphere::Update_Activating(const float ScaleMultiplier, bool CheckOverlaps)
{
	UpdateSphereRadius(ScaleMultiplier);

	if (CheckOverlaps)
	{
		//Check if we collide with the player
		TArray<AActor*> OverlappingActors;
		SphereCollisionComponent->UpdateOverlaps();
		SphereCollisionComponent->GetOverlappingActors(OverlappingActors, AQuantumFieldCharacter::StaticClass());
		if (OverlappingActors.Num() > 0)
		{
			InterruptActivate();
		}
	}
}

void AQuantumSphere::TryNotifyEndActivate()
{
	if (!NotifyEndActivateEnabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("SKIP TryNotifyEndActivate"));
		NotifyEndActivateEnabled = true;
		return;
	}
	NotifyEndActivate();
}

void AQuantumSphere::NotifyEndActivate()
{
	OnSphereFinishedActivating.Broadcast(SphereIndex, this);

	BP_SphereFinishedActivating();

	SphereMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	SphereMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bool bCurrentWorldState = false;
	if (const AQuantumFieldGameMode* GameMode = Cast<AQuantumFieldGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		bCurrentWorldState = GameMode->bWorldStateIsPast;
	}

	//Update Quantic state
	for (AQF_QuantumActor* Actor : QuantumActors)
	{
		Actor->ChangeState(!bCurrentWorldState, this);
		
		if(Actor->GetClass()->ImplementsInterface(UQF_IQuantumPickOject::StaticClass()))
		{
			IQF_IQuantumPickOject::Execute_EnterSphere(Actor);
		}
	}
}

/*
 * INTERRUPT
 */

void AQuantumSphere::InterruptActivate()
{
	FinalRadius = CurrentRadius;

	OnSphereFailedActivating.Broadcast(SphereIndex, this);
	
	BP_SphereInterruptActivate();
	
	UDynamicSphereManager::Get(GetWorld())->DespawnSphere(GetWorld(), this);
}

/*
 * DEACTIVATE
 */

void AQuantumSphere::SphereDeactivate()
{
	BP_SphereDeactivate();

	UDynamicSphereManager::Get(GetWorld())->DespawnSphere(GetWorld(), this);
}

void AQuantumSphere::Update_Deactivating(const float ScaleMultiplier)
{
	UpdateSphereRadius(ScaleMultiplier);
}

void AQuantumSphere::TryNotifyEndDeactivate()
{
	if (!NotifyEndDeactivateEnabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("SKIP TryNotifyEndDeactivate"));
		NotifyEndDeactivateEnabled = true;
		return;
	}
	NotifyEndDeactivate();
}

void AQuantumSphere::NotifyEndDeactivate()
{
	BP_SphereFinishedDeactivating();
	
	bool bCurrentWorldState = false;
	if (const AQuantumFieldGameMode* GameMode = Cast<AQuantumFieldGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		bCurrentWorldState = GameMode->bWorldStateIsPast;
	}

	for (AActor* Actor : QuantumActors)
	{
		Cast<AQF_QuantumActor>(Actor)->ChangeState(bCurrentWorldState, this);
				
		if(Actor->GetClass()->ImplementsInterface(UQF_IQuantumPickOject::StaticClass()))
		{
			IQF_IQuantumPickOject::Execute_TryReset(Actor);
		}
	}
}

void AQuantumSphere::NotifyDestroy()
{
	OnSphereFinishedDeactivating.Broadcast(SphereIndex, this);
	UpdateSphereRadius(0);
	Destroy();
}

int32 AQuantumSphere::GetSphereIndex() const
{
	return SphereIndex;
}

const FQF_BooleanModifier& AQuantumSphere::GetBooleanModifier() const
{
	return BooleanModifier;
}

/*
 * UPDATE RADIUS/SIZE
 */

void AQuantumSphere::UpdateSphereRadius(const float ScaleMultiplier)
{
	CurrentRadius = ScaleMultiplier * FinalRadius;

	SphereCollisionComponent->SetSphereRadius(CurrentRadius + 1, true);
	SphereMeshComponent->SetRelativeScale3D(FVector(ScaleMultiplier));
	UpdateMaterialRadius(CurrentRadius);
}

/*
 * UPDATE MATERIAL
 */

void AQuantumSphere::UpdateMaterialPosition(const FVector Position) const
{
	UMaterialParameterCollectionInstance* Instance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
	if (StaticSphere)
	{
		Instance->SetVectorParameterValue(*FString::Printf(TEXT("SP%d_Static_Position"), SphereIndex), Position);
	}
	else
	{
		Instance->SetVectorParameterValue(*FString::Printf(TEXT("SP%d_Position"), SphereIndex), Position);
	}
}

void AQuantumSphere::UpdateMaterialRadius(const float Radius) const
{
	UMaterialParameterCollectionInstance* Instance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
	if (StaticSphere)
	{
		Instance->SetScalarParameterValue(*FString::Printf(TEXT("SP%d_Static_CurrentRadius"), SphereIndex), Radius);
	}
	else
	{
		Instance->SetScalarParameterValue(*FString::Printf(TEXT("SP%d_CurrentRadius"), SphereIndex), Radius);
	}
}

void AQuantumSphere::UpdateMaterialSphereIndex(int InSphereIndex) const
{
	if (StaticSphere)
	{
		SphereMaterialInstance->SetScalarParameterValue("SphereIndex", InSphereIndex);
	}
	else
	{
		SphereMaterialInstance->SetScalarParameterValue("SphereIndex", InSphereIndex);
	}
}
