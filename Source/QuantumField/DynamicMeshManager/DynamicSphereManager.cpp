// Copyright Catness Game Studios. All Rights Reserved.

#include "DynamicSphereManager.h"

#include "Components/QF_QuantumMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "Interfaces/QF_IDynamicMeshManaged.h"
#include "Kismet/GameplayStatics.h"
#include "QuantumField/QuantumFieldGameMode.h"
#include "QuantumField/Actors/QF_QuantumActor.h"
#include "QuantumField/Actors/QuantumSphere.h"
#include "Tasks/ApplyMeshBooleanTask.h"
#include "Tasks/CopyStaticMeshTask.h"
#include "Tasks/NotifyEndTask.h"

FGraphEventRef UDynamicSphereManager::RequestCopyMesh(UQF_QuantumMeshComponent* Component, UStaticMesh* MeshToCopy, UDynamicMesh* TargetMesh, const FGraphEventRef& ParentTask)
{
	FGraphEventArray Prerequisites;
	Prerequisites.Add(ParentTask);
	FGraphEventRef Task = TGraphTask<FCopyStaticMeshTask>::CreateTask(&Prerequisites, ENamedThreads::AnyHiPriThreadHiPriTask)
		.ConstructAndDispatchWhenReady(Component, MeshToCopy, TargetMesh);
	return Task;
}

FGraphEventRef UDynamicSphereManager::RequestBoolean(UQF_QuantumMeshComponent* Component, bool bSubtract, UDynamicMesh* TargetMesh, const FGraphEventRef& ParentTask)
{
	FGraphEventArray Prerequisites;
	Prerequisites.Add(ParentTask);
	FGraphEventRef Task = TGraphTask<FApplyMeshBooleanTask>::CreateTask(&Prerequisites, ENamedThreads::AnyHiPriThreadHiPriTask)
		.ConstructAndDispatchWhenReady(Component, bSubtract, TargetMesh);
	return Task;
}

void UDynamicSphereManager::RequestNotifyFinish(UQF_QuantumMeshComponent* Component, const FGraphEventRef& ParentTask)
{
	RequestNotifyFinishPrerequisites.Add(ParentTask);
}

/*
 * POOL
 */

UDynamicMeshPool* UDynamicSphereManager::GetComputeMeshPool()
{
	if (DynamicMeshPool == nullptr && bEnableComputeMeshPool)
	{
		DynamicMeshPool = NewObject<UDynamicMeshPool>();
	}
	return DynamicMeshPool;
}

UDynamicMesh* UDynamicSphereManager::AllocateComputeMesh()
{
	if (bEnableComputeMeshPool)
	{
		UDynamicMeshPool* UsePool = GetComputeMeshPool();
		if (UsePool)
		{
			++MeshesInPool;
			++MeshesInUse;
			UE_LOG(LogTemp, Warning, TEXT("[AllocateComputeMesh] Pool: %d - Used: %d"), MeshesInPool, MeshesInUse);
			return UsePool->RequestMesh();
		}
	}

	// if we could not return a pool mesh, allocate a new mesh that isn't owned by the pool
	return NewObject<UDynamicMesh>(this);
}

bool UDynamicSphereManager::ReleaseComputeMesh(UDynamicMesh* Mesh)
{
	if (bEnableComputeMeshPool && Mesh)
	{
		UDynamicMeshPool* UsePool = GetComputeMeshPool();
		if (UsePool != nullptr)
		{
			--MeshesInUse;
			UE_LOG(LogTemp, Warning, TEXT("[ReleaseComputeMesh] Pool: %d - Used: %d"), MeshesInPool, MeshesInUse);
			UsePool->ReturnMesh(Mesh);
			return true;
		}
	}
	return false;
}

/*
 * MISC
 */

UDynamicMesh* UDynamicSphereManager::GetSpheresBoolean() const
{
	return CurrentSphereMesh;
}

void UDynamicSphereManager::SetSpheresBoolean(UDynamicMesh* DynamicMesh)
{
	CurrentSphereMesh = DynamicMesh;
}

const TArray<FQF_BooleanModifier>& UDynamicSphereManager::GetSpheresModifiers() const
{
	return SphereModifiers;
}

AQuantumSphere* UDynamicSphereManager::SpawnSphere(UWorld* World, const TSubclassOf<AQuantumSphere> SphereClass, const FVector Location)
{
	FTaskGraphInterface::Get().WaitUntilTasksComplete(RequestNotifyFinishPrerequisites);

	//Get the current world status
	bool bCurrentWorldState = false;
	if (const AQuantumFieldGameMode* GameMode = Cast<AQuantumFieldGameMode>(UGameplayStatics::GetGameMode(World)))
	{
		bCurrentWorldState = GameMode->bWorldStateIsPast;
	}

	//Spawn the actual Sphere
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AQuantumSphere* Sphere = World->SpawnActor<AQuantumSphere>(SphereClass, Location, FRotator::ZeroRotator, ActorSpawnParams);

	//Generate the Sphere mesh and set it
	UDynamicMesh* DynMesh = AllocateComputeMesh();
	DynMesh = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendSphereBox(
		DynMesh,
		FGeometryScriptPrimitiveOptions(),
		FTransform(),
		Sphere->SphereRadius,
		Sphere->SphereMeshSteps,
		Sphere->SphereMeshSteps,
		Sphere->SphereMeshSteps,
		EGeometryScriptPrimitiveOriginMode::Center);
	
	Sphere->BeginSphereActivate(DynMesh);

	const FQF_BooleanModifier& SpawnedSphereMod = Sphere->GetBooleanModifier();

	//Store the Sphere modifier for future operations
	AddNewSphere(SpawnedSphereMod);

	//Create the FApplyMeshBooleanTask
	TGraphTask<FApplySphereBooleanTask>* SphereBooleanTask = CreateBooleanSphereTask(SphereModifiers);

	RequestNotifyFinishPrerequisites.Reset();
	RequestNotifyFinishPrerequisites.Add(SphereBooleanTask->GetCompletionEvent());

	TArray<AQF_QuantumActor*> AffectedActors;
	TArray<IQF_IDynamicMeshManaged*> AffectedComponents;

	//Detect all components with which the sphere will collide
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Sphere->SphereRadius);
	TArray<FHitResult> OutResults;
	World->SweepMultiByChannel(OutResults, Location, Location, FQuat::Identity, ECC_WorldDynamic, CollisionShape);

	//We have to iterate actors and then components because the UQF_IDynamicMeshManaged meshes may not be regenerated
	//First, detect collided actors
	for (const FHitResult& Result : OutResults)
	{
		AQF_QuantumActor* Actor = Cast<AQF_QuantumActor>(Result.GetActor());
		if (Actor && !AffectedActors.Contains(Actor))
		{
			AffectedActors.Add(Actor);
		}
	}
	//Find the affected UQF_IDynamicMeshManaged components
	for (AQF_QuantumActor* Actor : AffectedActors)
	{
		TArray<USceneComponent*> Components;
		Actor->GetComponents<USceneComponent>(Components);
		for (USceneComponent* Component : Components)
		{
			if (Component->GetClass()->ImplementsInterface(UQF_IDynamicMeshManaged::StaticClass()))
			{
				IQF_IDynamicMeshManaged* Obj = Cast<IQF_IDynamicMeshManaged>(Component);
				Obj->SetupPrerequisites(!bCurrentWorldState, SphereBooleanTask->GetCompletionEvent());
				AffectedComponents.Add(Obj);
			}
		}
	}

	Sphere->QuantumActors = AffectedActors;

	FGraphEventRef Task = TGraphTask<FNotifyEndTask>::CreateTask(&RequestNotifyFinishPrerequisites, ENamedThreads::GameThread)
		.ConstructAndDispatchWhenReady(Sphere, AffectedComponents, true);

	if (NotifyTask.IsValid() && !NotifyTask->IsComplete())
	{
		NotifyTask->AddSubsequent(SphereBooleanTask);
	}
	else
	{
		SphereBooleanTask->Unlock();
	}
	NotifyTask = Task;

	return Sphere;
}

void UDynamicSphereManager::DespawnSphere(UWorld* World, AQuantumSphere* Sphere)
{
	if (Sphere)
	{
		FTaskGraphInterface::Get().WaitUntilTasksComplete(RequestNotifyFinishPrerequisites);

		//Get the current world status
		bool bCurrentWorldState = false;
		if (const AQuantumFieldGameMode* GameMode = Cast<AQuantumFieldGameMode>(UGameplayStatics::GetGameMode(World)))
		{
			bCurrentWorldState = GameMode->bWorldStateIsPast;
		}

		//Generate the Sphere mesh and set it
		const FQF_BooleanModifier& SpawnedSphereMod = Sphere->GetBooleanModifier();

		//Store the Sphere modifier for future operations
		DeleteSphere(SpawnedSphereMod);

		//Create the FApplyMeshBooleanTask
		TGraphTask<FApplySphereBooleanTask>* SphereBooleanTask = CreateBooleanSphereTask(SphereModifiers);

		RequestNotifyFinishPrerequisites.Reset();
		RequestNotifyFinishPrerequisites.Add(SphereBooleanTask->GetCompletionEvent());

		TArray<AQF_QuantumActor*> AffectedActors;
		TArray<IQF_IDynamicMeshManaged*> AffectedComponents;

		//Detect all components with wich the sphere will collide
		const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Sphere->SphereRadius);
		TArray<FHitResult> OutResults;
		World->SweepMultiByChannel(OutResults, Sphere->GetActorLocation(), Sphere->GetActorLocation(), FQuat::Identity, ECC_WorldDynamic, CollisionShape);

		//First, detect collided actors
		for (const FHitResult& Result : OutResults)
		{
			AQF_QuantumActor* Actor = Cast<AQF_QuantumActor>(Result.GetActor());
			if (Actor && !AffectedActors.Contains(Actor))
			{
				AffectedActors.Add(Actor);
			}
		}
		//Find the affected UQF_IDynamicMeshManaged components
		for (AQF_QuantumActor* Actor : AffectedActors)
		{
			TArray<USceneComponent*> Components;
			Actor->GetComponents<USceneComponent>(Components);
			for (USceneComponent* Component : Components)
			{
				if (Component->GetClass()->ImplementsInterface(UQF_IDynamicMeshManaged::StaticClass()))
				{
					IQF_IDynamicMeshManaged* Obj = Cast<IQF_IDynamicMeshManaged>(Component);
					Obj->SetupPrerequisites(!bCurrentWorldState, SphereBooleanTask->GetCompletionEvent());
					AffectedComponents.Add(Obj);
				}
			}
		}

		FGraphEventRef Task = TGraphTask<FNotifyEndTask>::CreateTask(&RequestNotifyFinishPrerequisites, ENamedThreads::GameThread)
			.ConstructAndDispatchWhenReady(Sphere, AffectedComponents, false);

		if (NotifyTask.IsValid() && !NotifyTask->IsComplete())
		{
			NotifyTask->AddSubsequent(SphereBooleanTask);
		}
		else
		{
			SphereBooleanTask->Unlock();
		}
		NotifyTask = Task;
	}
}

void UDynamicSphereManager::AddNewSphere(const FQF_BooleanModifier& Modifier)
{
	CurrentSphereMesh = AllocateComputeMesh();

	SphereModifiers.Add(Modifier);
}

void UDynamicSphereManager::DeleteSphere(const FQF_BooleanModifier& Modifier)
{
	SphereModifiers.Remove(Modifier);

	CurrentSphereMesh = AllocateComputeMesh();
}

TGraphTask<FApplySphereBooleanTask>* UDynamicSphereManager::CreateBooleanSphereTask(TArray<FQF_BooleanModifier> TaskModifiers)
{
	return TGraphTask<FApplySphereBooleanTask>::CreateTask(nullptr, ENamedThreads::AnyHiPriThreadHiPriTask)
		.ConstructAndHold(this, CurrentSphereMesh, TaskModifiers);
}

UDynamicSphereManager* UDynamicSphereManager::Get(const UObject* WorldContextObject)
{
	AQuantumFieldGameMode* GameMode = Cast<AQuantumFieldGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (WorldContextObject && GameMode)
	{
		if (!GameMode->DynamicSphereManager)
		{
			GameMode->DynamicSphereManager = NewObject<UDynamicSphereManager>();
		}
		return GameMode->DynamicSphereManager;
	}
	return nullptr;
}
