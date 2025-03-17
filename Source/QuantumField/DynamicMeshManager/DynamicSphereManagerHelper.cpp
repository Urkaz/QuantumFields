// Copyright Catness Game Studios. All Rights Reserved.

#include "DynamicSphereManagerHelper.h"

#include "DynamicSphereManager.h"
#include "Components/DynamicMeshComponent.h"

float squared(float x) { return x * x; }

// Computes the square distance between a point p and an AABB b
float SqDistPointAABB(FVector Point, FVector BoxMin, FVector BoxMax)
{
	float sqDist = 0.0f;
	// For each axis count any excess distance outside box extents
	//float v = ;
	if (Point.X < BoxMin.X) sqDist += squared(BoxMin.X - Point.X);
	else if (Point.X > BoxMax.X) sqDist += squared(Point.X - BoxMax.X);
	if (Point.Y < BoxMin.Y) sqDist += squared(BoxMin.Y - Point.Y);
	else if (Point.Y > BoxMax.Y) sqDist += squared(Point.Y - BoxMax.Y);
	if (Point.Z < BoxMin.Z) sqDist += squared(BoxMin.Z - Point.Z);
	else if (Point.Z > BoxMax.Z) sqDist += squared(Point.Z - BoxMax.Z);
	return sqDist;
}

// Returns true if sphere intersects AABB, false otherwise
bool TestSphereAABBOverlap(FVector Center, float Radius, FVector BoxMin, FVector BoxMax)
{
	// Compute squared distance between sphere center and AABB
	float sqDist = SqDistPointAABB(Center, BoxMin, BoxMax);
	// Sphere and AABB intersect if the (squared) distance
	// between them is less than the (squared) sphere radius
	return sqDist <= squared(Radius);
}

bool UDynamicSphereManagerHelper::IsMeshInsideSpheres(const UMeshComponent* Mesh)
{
	const UDynamicSphereManager* DynMeshMgr = UDynamicSphereManager::Get(Mesh->GetWorld());
	const TArray<FQF_BooleanModifier>& Spheres = DynMeshMgr->GetSpheresModifiers();

	if (Spheres.Num() == 0)
	{
		return false;
	}

	const FVector Origin = Mesh->Bounds.Origin;
	const FVector Extent = Mesh->Bounds.BoxExtent;

	constexpr int32 NumberOfVertices = 8;
	TArray<bool> VerticesInside;
	VerticesInside.AddDefaulted(NumberOfVertices);

	//For each sphere we check if the vertices forming BoundingBox of this mesh are inside them.
	//The vertices may not be all inside the SAME sphere, but they may be inside the combination of multiple ones.
	for (int i = 0; i < Spheres.Num(); ++i)
	{
		const FVector SphereOirign = Spheres[i].Transform.GetLocation();
		const float SphereRadius = Spheres[i].SphereRadius;

		int32 VertexIndex = 0;

		//Iterate -1 and 1
		for (int x = -1; x <= 1; x += 2)
		{
			for (int y = -1; y <= 1; y += 2)
			{
				for (int z = -1; z <= 1; z += 2)
				{
					FVector Point = Origin + FVector(x * Extent.X, y * Extent.Y, z * Extent.Z);

					const float Dist = FVector::DistSquared(SphereOirign, Point);
					VerticesInside[VertexIndex] |= Dist < SphereRadius * SphereRadius;

					++VertexIndex;
				}
			}
		}
	}

	bool IsInside = true;
	for (int i = 0; i < NumberOfVertices; ++i)
	{
		IsInside &= VerticesInside[i];
	}
	return IsInside;
}

bool UDynamicSphereManagerHelper::IsMeshOutsideAllSpheres(const UMeshComponent* Mesh)
{
	const UDynamicSphereManager* DynMeshMgr = UDynamicSphereManager::Get(Mesh->GetWorld());
	const TArray<FQF_BooleanModifier>& Spheres = DynMeshMgr->GetSpheresModifiers();

	if (Spheres.Num() == 0)
	{
		return true;
	}

	const FVector Origin = Mesh->Bounds.Origin;
	const FVector Extent = Mesh->Bounds.BoxExtent;

	for (int i = 0; i < Spheres.Num(); ++i)
	{
		const FVector SphereOirign = Spheres[i].Transform.GetLocation();
		const float SphereRadius = Spheres[i].SphereRadius;

		if (TestSphereAABBOverlap(SphereOirign, SphereRadius, Origin - Extent, Origin + Extent))
			return false;
	}

	return true;
}
