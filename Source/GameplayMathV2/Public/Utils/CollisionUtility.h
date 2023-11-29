#pragma once

#include "CoreMinimal.h"
#include "Structs/Colliders/CollisionHit.h"
#include "CollisionUtility.generated.h"

class UCollisionComponent;
class AMathActor;
struct FCollider;

UCLASS()
class GAMEPLAYMATHV2_API UCollisionUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static bool PlanePlaneCollision( const FVector& NormalA, const FVector2d BoundsA,
		const FVector& NormalB, const FVector2d BoundsB, FCollisionHit& CollisionHit);

	static bool SpherePlaneCollision( const FVector& SphereCenter, const float SphereRadius,
		const FVector& PlaneCenter, const FVector& PlaneNormal, const FVector2d& PlaneBounds, FCollisionHit& CollisionHit);

	static bool SphereSphereCollision(const FVector& CenterA, const float RadiusA, const FVector& CenterB,
		const float RadiusB, FCollisionHit& CollisionHit);

	static bool RaySphereCollision(const FVector& Origin, const FVector& RayDirection, const float RayLength,
		const FVector& SphereCenter, const float SphereRadius, FCollisionHit& CollisionHit);

	static bool RayPlaneCollision(const FVector& RayOrigin, const FVector& RayDirection, const float RayLength,
		const FVector& PlaneCenter, const FVector& PlaneNormal, FCollisionHit& CollisionHit);

	static bool RayAABBCollision(const FVector& RayOrigin, const FVector& RayDirection, const float RayLength,
		const FVector& Min, const FVector& Max, FCollisionHit& CollisionHit);
	
	static bool AABBAABBCollision(const FVector& MinA, const FVector& MaxA, const FVector& MinB,
		const FVector& MaxB, FCollisionHit& CollisionHit);

	static bool PointAABBCollision(const FVector& Point, const FVector& Min, const FVector& Max,
		FCollisionHit& CollisionHit);

	static bool SphereAABBCollision(const FVector& SphereCenter, const float SphereRadius,
		const FVector& Min, const FVector& Max, FCollisionHit& CollisionHit);

	static bool AABBPlaneCollision(const FVector& AABBLocation, const FVector& AABBExtent, const FTransform& PlaneTransform,
								  const FVector& PlaneNormal, const FVector2D PlaneExtent, FCollisionHit& CollisionHit);
	
	// Ray-Triangle Intersection
	static bool RayTriangle(const FVector& Origin, const FVector& Direction, const FVector& V0, const FVector& V1, const FVector& V2,
		FCollisionHit& ContactPoint);
	
	static bool SphereTriangleCollision(const FVector& SphereCenter, float SphereRadius, const FVector& V0, const FVector& V1,
		const FVector& V2, FCollisionHit& CollisionHit);
};