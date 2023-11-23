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
	
	static bool AABBCollision(const FVector& MinA, const FVector& MaxA, const FVector& MinB,
		const FVector& MaxB, FCollisionHit& CollisionHit);

	static bool PointAABBCollision(const FVector& Point, const FVector& Min, const FVector& Max,
		FCollisionHit& CollisionHit);

	static bool SphereAABBCollision(const FVector& SphereCenter, const float SphereRadius,
		const FVector& Min, const FVector& Max, FCollisionHit& CollisionHit);

	static bool AABBPlaneIntersection(const FVector& AABBLocation, const FVector& AABBExtent, const FTransform& PlaneTransform,
                                  const FVector& PlaneNormal, const FVector2D PlaneExtent, FCollisionHit& CollisionHit);
	
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
	
};
