#pragma once

#include "CoreMinimal.h"
#include "Structs/Colliders/CollisionHit.h"
#include "CollisionSubsystem.generated.h"

class UCollisionComponent;
struct FCollisionPoint;

UCLASS()
class GAMEPLAYMATHV2_API UCollisionSubsystem: public UTickableWorldSubsystem
{
public:
	GENERATED_BODY()

	UCollisionSubsystem();
	virtual TStatId GetStatId() const override;

	virtual void Tick(float DeltaTime) override;

	void ClearAllCollisionsFromComponents();
	bool RayCast(const FVector& RayOrigin, const FVector& RayDirection, float RayLength, FCollisionHit& CollisionHit);
	bool TestCollision(const UCollisionComponent* Component, FCollisionHit& CollisionHit);
	void Register(UCollisionComponent* Component) { CollisionComponents.AddUnique(Component); }
	void Unregister(UCollisionComponent* Component) { CollisionComponents.Remove(Component); }

private:
	UPROPERTY()
	TArray<UCollisionComponent*> CollisionComponents;
	
};