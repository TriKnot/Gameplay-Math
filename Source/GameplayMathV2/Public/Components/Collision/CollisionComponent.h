#pragma once

#include "CoreMinimal.h"
#include "Structs/Colliders/CollisionHit.h"
#include "Structs/Colliders/Collider.h"
#include "CollisionComponent.generated.h"

class UMathMovementComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories=(Variable, Sockets, Tags, ComponentTick, ComponentReplication, Activation, Cooking, Events, AssetUserData, Replication, Navigation))
class UCollisionComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void HandleCollisions();
	void HandleCollision(FCollisionHit& CollisionHit) const;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual const FCollider& GetCollider() const
	{
		return Collider;
	}

	void AddCollision(const FCollisionHit& Collision) { Collisions.Add(Collision); }
	void ClearCollisions() { Collisions.Empty(); }

	bool BHasCollided() const { return Collisions.Num() > 0; }

	virtual void DrawDebugCollider() const;

	virtual bool TestCollision(const UCollisionComponent* Other, FCollisionHit& OutCollision) const
	{
		return Collider.TestCollision(GetOwner()->GetActorTransform(), Other->GetCollider(), Other->GetOwner()->GetActorTransform(), OutCollision);
	}


public:
	bool bIsTrigger;

protected:
	FCollider Collider;

	TArray<FCollisionHit> Collisions;
	
	UPROPERTY(EditAnywhere, meta=( AllowPrivateAccess = "true" ))
	bool bIsKinematic;

	UPROPERTY(EditAnywhere, meta=( AllowPrivateAccess = "true" ))
	bool bIsCollisionActive;
	
	UPROPERTY()
	UMathMovementComponent* MovementComponent;

	
	
};
