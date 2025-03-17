// Fill out your copyright notice in the Description page of Project Settings.


#include "QF_SphereProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "DynamicMeshManager/DynamicSphereManager.h"

// Sets default values
AQF_SphereProjectile::AQF_SphereProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AQF_SphereProjectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	InitialLifeSpan = 1.0f;
}

// Called when the game starts or when spawned
void AQF_SphereProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AQF_SphereProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!bAlreadySpawnedSphere) 
	{
		OnSphereSpawned.Broadcast(nullptr);
	}
}

// Called every frame
void AQF_SphereProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQF_SphereProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherComp && OtherActor != this)
	{
		AQuantumSphere* HitSphere = Cast<AQuantumSphere>(OtherActor);
		if (!HitSphere)
		{
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AQuantumSphere* QuantumSphere = UDynamicSphereManager::Get(GetWorld())->SpawnSphere(GetWorld(), SphereClass, HitComp->GetComponentLocation());
				SpawnQuantumSphere(QuantumSphere);
			}
		}
		else 
		{
			//OnSphereSpawned.Broadcast(nullptr);
			OnSphereDestroyed.Broadcast(HitSphere);
			bAlreadySpawnedSphere = true;
		}

		Destroy();
	}
}

void AQF_SphereProjectile::SpawnQuantumSphere(AQuantumSphere* Sphere)
{
	OnSphereSpawned.Broadcast(Sphere);
	bAlreadySpawnedSphere = true;
}

