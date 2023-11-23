#pragma once
#include "CoreMinimal.h"
#include "Collider.h"
#include "SphereCollider.generated.h"

USTRUCT(BlueprintType)
struct FSphereCollider : public FCollider
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;

	virtual EColliderType GetType() const override
	{
		return EColliderType::Sphere;
	}
	
	virtual bool TestCollision(
		const FTransform Transform,
		const FCollider& OtherCollider,
		const FTransform OtherTransform,
		FCollisionHit& CollisionPoint) const override;

	virtual bool TestCollision(
		const FTransform Transform,
		const FSphereCollider& OtherCollider,
		const FTransform OtherTransform,
		FCollisionHit& CollisionPoint) const override;

	
	virtual bool TestCollision(
		const FTransform Transform,
		const FPlaneCollider& OtherCollider,
		const FTransform OtherTransform,
		FCollisionHit& CollisionPoint) const override;
	
	virtual bool TestCollision(
	const FTransform Transform,
	const FBoxCollider& OtherCollider,
	const FTransform OtherTransform,
	FCollisionHit& CollisionPoint) const override;

	virtual FVector GetBoundaryOffset(const FVector& Vector) const override
	{
		return Vector.GetSafeNormal() * Radius;
	}

};
