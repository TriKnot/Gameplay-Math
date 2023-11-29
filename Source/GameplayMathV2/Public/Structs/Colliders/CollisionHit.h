#pragma once

#include "CoreMinimal.h"
#include "CollisionHit.generated.h"


USTRUCT(BlueprintType)
struct FCollisionHit
{
	GENERATED_BODY()
	FVector CollisionPoint; // Point of collision
	FVector CollisionNormal; // Normal of the collision
	float Depth; // Length of B - A

	TArray<FVector> TestPoints;

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
