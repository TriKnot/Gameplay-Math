#include "Structs/Colliders/BoxCollider.h"

#include "Structs/Colliders/SphereCollider.h"
#include "Structs/Colliders/PlaneCollider.h"
#include "Utils/CollisionUtility.h"

bool FBoxCollider::TestCollision(const FTransform Transform, const FCollider& OtherCollider,
                                 const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	switch (OtherCollider.GetType())
	{
	case EColliderType::Sphere:
		return TestCollision(Transform, static_cast<const FSphereCollider&>(OtherCollider), OtherTransform, CollisionPoint);
	case EColliderType::Plane:
		return TestCollision(Transform, static_cast<const FPlaneCollider&>(OtherCollider), OtherTransform, CollisionPoint);
	case EColliderType::Box:
		return TestCollision(Transform, static_cast<const FBoxCollider&>(OtherCollider), OtherTransform, CollisionPoint);
	default:
		break;
	}
	return false;
}

bool FBoxCollider::TestCollision(const FTransform Transform, const FSphereCollider& OtherCollider,
	const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	return UCollisionUtility::SphereAABBCollision(OtherTransform.GetLocation(), OtherCollider.Radius, Transform.GetLocation(), Extent, CollisionPoint);
}

bool FBoxCollider::TestCollision(const FTransform Transform, const FPlaneCollider& OtherCollider,
	const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	return UCollisionUtility::AABBPlaneCollision(Transform.GetLocation(), Extent, OtherTransform, OtherTransform.GetRotation().GetForwardVector(), OtherCollider.Extent, CollisionPoint);
}

bool FBoxCollider::TestCollision(const FTransform Transform, const FBoxCollider& OtherCollider,
	const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	const FVector MinA = Transform.GetLocation() - Extent;
	const FVector MaxA = Transform.GetLocation() + Extent;
	const FVector MinB = OtherTransform.GetLocation() - OtherCollider.Extent;
	const FVector MaxB = OtherTransform.GetLocation() + OtherCollider.Extent;
	return UCollisionUtility::AABBAABBCollision(MinA, MaxA, MinB, MaxB, CollisionPoint);
}

bool FBoxCollider::TestCollision(const FTransform Transform, ANoiseFloor* Other,
	FCollisionHit& CollisionPoint) const
{
	// TODO: Implement
	return false;
}
