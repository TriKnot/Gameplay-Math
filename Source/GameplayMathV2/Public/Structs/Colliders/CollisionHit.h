#pragma once

struct FCollisionHit
{
	FVector CollisionPoint; // Point of collision
	FVector CollisionNormal; // Normal of the collision
	float Depth; // Length of B - A

	// For testing
	FVector A;
	FVector B;

	FCollisionHit()
		: CollisionPoint(FVector::ZeroVector)
		, CollisionNormal(FVector::ZeroVector)
		, Depth(0.0f)
	{
	}
};
