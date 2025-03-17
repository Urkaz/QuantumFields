// Fill out your copyright notice in the Description page of Project Settings.


#include "QF_WeaponComponent.h"
#include "QuantumFieldCharacter.h"
#include "QF_SphereProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UQF_WeaponComponent::UQF_WeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UQF_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	bCanShoot = true;

	for (int i = 0; i < MaxSpheres; ++i)
	{
		IndexesArray.Add(i);
	}
}


// Called every frame
void UQF_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQF_WeaponComponent::Fire()
{
	if (!Character) return;

	if (bCanShoot && !PlayerIsInsideSphere() && !Character->IsBusy())
	{
		//Delete the oldes sphere
		if (SpawnedSpheres.Num() == MaxSpheres)
		{
			//RemoveSphere(FindFirstSphereIndex());
			return;
		}

		bCanShoot = false;

		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(ShootingOffset);

		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AQF_SphereProjectile* Projectile = World->SpawnActor<AQF_SphereProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			Projectile->OnSphereSpawned.AddDynamic(this, &UQF_WeaponComponent::OnSphereSpawned);
			Projectile->OnSphereDestroyed.AddDynamic(this, &UQF_WeaponComponent::OnSphereDestroyed);

			if (FireSound)
			{
				//TODO: Add sound effect to the fire action
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetComponentLocation(), FRotator::ZeroRotator);
			}

			if (FireAnimation)
			{
				//TODO: Add animation to the fire action
			}
		}
	}
}

void UQF_WeaponComponent::RecoverSphere()
{
	if (!bCanShoot)
		return;

	const int32 RecoverSphereIndex = FindFirstSphereIndex();

	if (RecoverSphereIndex == -1 || PlayerIsInsideSphere(RecoverSphereIndex))
		return;

	RemoveSphere(RecoverSphereIndex);

	if (RecoverSound)
	{
		//TODO: Add sound effect to the recover action
	}

	if (RecoverAnimation)
	{
		//TODO: Add animation to the recover action
	}
}

void UQF_WeaponComponent::RemoveSphere(const int32 Index, bool Interrupted)
{
	AQuantumSphere* SphereToRemove = nullptr;
	for (AQuantumSphere* Sphere : SpawnedSpheres)
	{
		if (Sphere->GetSphereIndex() == Index)
		{
			SphereToRemove = Sphere;
			break;
		}
	}

	if (SphereToRemove)
	{
		bCanShoot = false;
		if (!Interrupted)
		{
			SphereToRemove->SphereDeactivate();
		}
		SpawnedSpheres.Remove(SphereToRemove);
		IndexesArray.Add(Index);
	}
}

void UQF_WeaponComponent::AddSphere(AQuantumSphere* NewSphere)
{
	NewSphere->SphereActivate(IndexesArray[0]);
	NewSphere->OnSphereFinishedActivating.AddDynamic(this, &UQF_WeaponComponent::OnSphereFinishedActivating);
	NewSphere->OnSphereFailedActivating.AddDynamic(this, &UQF_WeaponComponent::OnSphereFailedActivating);
	NewSphere->OnSphereFinishedDeactivating.AddDynamic(this, &UQF_WeaponComponent::OnSphereFinishedDeactivating);

	SpawnedSpheres.Add(NewSphere);
	IndexesArray.RemoveAt(0);
}

bool UQF_WeaponComponent::PlayerIsInsideSphere(int32 SphereIndex)
{
	TArray<AActor*> OverlapResult;
	Character->GetOverlappingActors(OverlapResult, AQuantumSphere::StaticClass());

	//SphereIndex = -1 means that you just wanna check if the player is inside any of the spheres
	if (SphereIndex == -1)
		return OverlapResult.Num() > 0;

	for (AQuantumSphere* Sphere : SpawnedSpheres)
	{
		if (Sphere->GetSphereIndex() == SphereIndex && OverlapResult.Contains(Sphere))
			return true;
	}
	return false;
}

int32 UQF_WeaponComponent::FindFirstSphereIndex()
{
	if (SpawnedSpheres.Num() > 0 && SpawnedSpheres[0])
		return SpawnedSpheres[0]->GetSphereIndex();
	return -1;
}

void UQF_WeaponComponent::OnSphereSpawned(AQuantumSphere* Sphere)
{
	if (Sphere)
	{
		AddSphere(Sphere);
	}
	else
	{
		bCanShoot = true;
	}
}

void UQF_WeaponComponent::OnSphereDestroyed(AQuantumSphere* Sphere)
{
	int32 Index = Sphere->GetSphereIndex();
	RemoveSphere(Index);
}

void UQF_WeaponComponent::OnSphereFinishedActivating(int32 SphereIndex, AQuantumSphere* Ptr)
{
	bCanShoot = true;
}

void UQF_WeaponComponent::OnSphereFailedActivating(int32 SphereIndex, AQuantumSphere* Ptr)
{
	RemoveSphere(SphereIndex, true);
}

void UQF_WeaponComponent::OnSphereFinishedDeactivating(int32 SphereIndex, AQuantumSphere* Ptr)
{
	bCanShoot = true;
}

void UQF_WeaponComponent::AttachWeapon(AQuantumFieldCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	if (!Character || Character->GetHasRifle())
		return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	Character->SetHasRifle(true);

	Character->SetQuantumWeapon(this);

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UQF_WeaponComponent::Fire);
			EnhancedInputComponent->BindAction(RecoverAction, ETriggerEvent::Triggered, this, &UQF_WeaponComponent::RecoverSphere);
		}
	}
}
