#pragma once

#include "CoreMinimal.h"
#include "Actor/NoiseFloor.h"
#include "Structs/Colliders/CollisionHit.h"
#include "CollisionSubsystem.generated.h"

class UCollisionComponent;
class ANoiseFloor;
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
	bool TestCollision(UCollisionComponent* Component, FCollisionHit& CollisionHit);
	void Register(UCollisionComponent* Component) { CollisionComponents.AddUnique(Component); }
	void Unregister(UCollisionComponent* Component) { CollisionComponents.Remove(Component); }
	void RegisterNoiseFloor(ANoiseFloor* InNoiseFloor) { NoiseFloor = InNoiseFloor; }
	void UnregisterNoiseFloor() { NoiseFloor = nullptr; }
	bool TestAgainstFloor(UCollisionComponent* Component, FCollisionHit& CollisionHit) const;

private:
	UPROPERTY()
	TArray<UCollisionComponent*> CollisionComponents;
	UPROPERTY()
	ANoiseFloor* NoiseFloor;
	
};