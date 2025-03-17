// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/QuantumSphere.h"
#include "QF_SphereProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuantumSphereSpawned, AQuantumSphere*, SphereSpawned);

UCLASS()
class QUANTUMFIELD_API AQF_SphereProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQF_SphereProjectile();

	FQuantumSphereSpawned OnSphereSpawned;
	FQuantumSphereSpawned OnSphereDestroyed;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AQuantumSphere> SphereClass;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	bool bAlreadySpawnedSphere{ false };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SpawnQuantumSphere(AQuantumSphere* Sphere);
};
