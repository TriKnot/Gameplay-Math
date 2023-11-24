#pragma once
#include "CoreMinimal.h"
#include "Collider.generated.h"

class ANoiseFloor;
struct FBoxCollider;
struct FCollisionHit;
struct FCollisionPoint;
struct FPlaneCollider;
struct FSphereCollider;

UENUM(BlueprintType)
enum class EColliderType {
	None,
	Sphere,
	Plane,
	Box
};

USTRUCT(BlueprintType)
struct FCollider
{
	GENERATED_BODY()

	virtual EColliderType GetType() const
	{
		return EColliderType::None;
	}
	
	virtual bool TestCollision(
		const FTransform Transform,
		const FCollider& OtherCollider,
		const FTransform OtherTransform,
		FCollisionHit& CollisionPoint) const;
	
	virtual bool TestCollision(
		const FTransform Transform,
		const FSphereCollider& OtherCollider,
		const FTransform OtherTransform,
		FCollisionHit& CollisionPoint) const;

	
	virtual bool TestCollision(
		const FTransform Transform,
		const FPlaneCollider& OtherCollider,
		const FTransform OtherTransform,
		FCollisionHit& CollisionPoint) const;
	
	virtual bool TestCollision(
	const FTransform Transform,
	const FBoxCollider& OtherCollider,
	const FTransform OtherTransform,
	FCollisionHit& CollisionPoint) const;

	virtual bool TestCollision(
		const FTransform Transform,
		ANoiseFloor* Other,
		FCollisionHit& CollisionPoint) const;
	
	virtual ~FCollider() {}
	
	virtual FVector GetBoundaryOffset(const FVector& Vector) const { return FVector::ZeroVector; }
	
};
