#pragma once
#include "CoreMinimal.h"
#include "Collider.h"
#include "BoxCollider.generated.h"

USTRUCT(BlueprintType)
struct FBoxCollider : public FCollider
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Extent;

	
	virtual EColliderType GetType() const override
	{
		return EColliderType::Box;
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

	virtual bool TestCollision(
	const FTransform Transform,
	ANoiseFloor* Other,
	FCollisionHit& CollisionPoint) const override;

	
};

