#include "Structs/Colliders/Collider.h"

#include "Structs/Colliders/BoxCollider.h"
#include "Structs/Colliders/PlaneCollider.h"
#include "Structs/Colliders/SphereCollider.h"

bool FCollider::TestCollision(const FTransform Transform, const FCollider& OtherCollider,
                              const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	return false;
}

bool FCollider::TestCollision(const FTransform Transform, const FSphereCollider& OtherCollider,
	const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	return false;
}

bool FCollider::TestCollision(const FTransform Transform, const FPlaneCollider& OtherCollider,
	const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	return false;
}

bool FCollider::TestCollision(const FTransform Transform, const FBoxCollider& OtherCollider,
	const FTransform OtherTransform, FCollisionHit& CollisionPoint) const
{
	return false;
}


