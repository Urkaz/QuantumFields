// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/QuantumSphere.h"
#include "QF_WeaponComponent.generated.h"

class AQF_SphereProjectile;
class AQuantumFieldCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUANTUMFIELD_API UQF_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQF_WeaponComponent();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AQF_SphereProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* RecoverSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* RecoverAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector ShootingOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	int32 MaxSpheres{ 1 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponData")
	bool bCanShoot{ true };

	UPROPERTY(BlueprintReadOnly, Category = "WeaponData")
	TArray<AQuantumSphere*> SpawnedSpheres;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponData")
	TArray<int32> IndexesArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RecoverAction;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//TODO: Change class type with the final character class
	UPROPERTY()
	AQuantumFieldCharacter* Character;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RecoverSphere();

	UFUNCTION(BlueprintCallable, Category = "Attachments")
	void AttachWeapon(AQuantumFieldCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Collisions")
	bool PlayerIsInsideSphere(int32 SphereIndex = -1);

protected:

	UFUNCTION()
	void OnSphereSpawned(AQuantumSphere* Sphere);
	UFUNCTION()
	void OnSphereDestroyed(AQuantumSphere* Sphere);

	UFUNCTION()
	void OnSphereFinishedActivating(int32 SphereIndex, AQuantumSphere* Ptr);
	UFUNCTION()
	void OnSphereFinishedDeactivating(int32 SphereIndex, AQuantumSphere* Ptr);
	UFUNCTION()
	void OnSphereFailedActivating(int32 SphereIndex, AQuantumSphere* Ptr);

	void RemoveSphere(int32 Index, bool Interrupted = false);

	void AddSphere(AQuantumSphere* NewSphere);
	
	int32 FindFirstSphereIndex();
};
