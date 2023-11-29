#pragma once

#include "CoreMinimal.h"
#include "Structs/Colliders/CollisionHit.h"
#include "IntersectionUtility.generated.h"

class UCollisionComponent;
class AMathActor;
struct FCollider;

UCLASS()
class GAMEPLAYMATHV2_API UIntersectionUtility : public UBlueprintFunctionLibrary
{
GENERATED_BODY()
public:
	static bool PlanePlaneIntersection( const FVector& NormalA, const FVector2d BoundsA,
		const FVector& NormalB, const FVector2d BoundsB);

	static bool SpherePlaneIntersection( const FVector& SphereCenter, const float SphereRadius,
		const FVector& PlaneCenter, const FVector& PlaneNormal, const FVector2d& PlaneBounds);

	static bool SphereSphereIntersection(const FVector& CenterA, const float RadiusA, const FVector& CenterB,
		const float RadiusB);

	static bool RaySphereIntersection(const FVector& Origin, const FVector& RayDirection, const float RayLength,
		const FVector& SphereCenter, const float SphereRadius);

	static bool RayPlaneIntersection(const FVector& RayOrigin, const FVector& RayDirection, const float RayLength,
		const FVector& PlaneCenter, const FVector& PlaneNormal);

	static bool RayAABBIntersection(const FVector& RayOrigin, const FVector& RayDirection, const float RayLength,
		const FVector& Min, const FVector& Max);
	
	static bool AABBAABBIntersection(const FVector& MinA, const FVector& MaxA, const FVector& MinB,
		const FVector& MaxB);

	static bool PointAABBIntersection(const FVector& Point, const FVector& Min, const FVector& Max);

	static bool SphereAABBIntersection(const FVector& SphereCenter, const float SphereRadius, const FVector& AABBCenter, const FVector& AABBExtents, FVector& OutClosestPoint);

	static bool AABBPlaneIntersection(const FVector& AABBLocation, const FVector& AABBExtent, const FTransform& PlaneTransform,
                                  const FVector& PlaneNormal, const FVector2D PlaneExtent);
	
	static bool IsOnScreen(const APlayerController* PlayerController, const UCollisionComponent* Component,
                       const float ScreenSpaceCompare = 0.75f);
	
	static bool IsPointOnScreen(const APlayerController* PlayerController, const FVector& WorldPosition,
		const float ScreenSpaceCompare = 0.75f);

	static bool IsAABBOnScreen(const FVector& Min, const FVector& Max, const APlayerController* PlayerController,
		const float ScreenCompare = 0.75f);

	static bool IsSphereOnScreen(const FVector& Center, const float Radius, const APlayerController* PlayerController,
		const float ScreenCompare = 0.75f);

	static bool IsPlaneOnScreen(const FVector& Center, const FVector& Normal, const FVector2d Bounds,
		const APlayerController* PlayerController, const float ScreenCompare = 0.75f);

	static bool RayTriangleIntersection(const FVector& Origin, const FVector& Direction, const FVector& V0, const FVector& V1, const FVector& V2);
	
	static bool SphereTriangleIntersection(const FVector& SphereCenter, float SphereRadius, const FVector& V0, const FVector& V1,
		const FVector& V2);

	static bool PointTriangleIntersection(const FVector& Point, const FVector& V0, const FVector& V1, const FVector& V2);
	
};
