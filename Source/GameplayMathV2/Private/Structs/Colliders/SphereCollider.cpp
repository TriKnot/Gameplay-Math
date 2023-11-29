#include "Structs/Colliders/SphereCollider.h"

#include "Structs/Colliders/BoxCollider.h"
#include "Structs/Colliders/PlaneCollider.h"
#include "Utils/CollisionUtility.h"
#include "Actor/NoiseFloor.h"

bool FSphereCollider::TestCollision(const FTransform Transform, const FCollider& OtherCollider,
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

bool FSphereCollider::TestCollision(const FTransform Transform, const FSphereCollider& OtherCollider,
	const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	return UCollisionUtility::SphereSphereCollision(Transform.GetLocation(), Radius, OtherTransform.GetLocation(), OtherCollider.Radius, CollisionPoint);
}

bool FSphereCollider::TestCollision(const FTransform Transform, const FPlaneCollider& OtherCollider,
	const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	return UCollisionUtility::SpherePlaneCollision(Transform.GetLocation(), Radius, OtherTransform.GetLocation(), OtherTransform.GetRotation().GetForwardVector(), OtherCollider.Extent, CollisionPoint);
}

bool FSphereCollider::TestCollision(const FTransform Transform, const FBoxCollider& OtherCollider,
	const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	return UCollisionUtility::SphereAABBCollision(Transform.GetLocation(), Radius, OtherTransform.GetLocation(), OtherCollider.Extent, CollisionPoint);
}

bool FSphereCollider::TestCollision(const FTransform Transform, ANoiseFloor* Other,
	FCollisionHit& CollisionPoint) const
{
	// Get closest point on noise floor
	TArray<FVector> TrianglePoints;
	Other->FindClosestTriangle(Transform.GetLocation(), TrianglePoints);
	
	return UCollisionUtility::SphereTriangleCollision(Transform.GetLocation(), Radius, TrianglePoints[0], TrianglePoints[1], TrianglePoints[2], CollisionPoint);
}
