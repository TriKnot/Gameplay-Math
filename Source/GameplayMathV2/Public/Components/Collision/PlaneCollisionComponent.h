#pragma once
#include "CollisionComponent.h"
#include "Structs/Colliders/PlaneCollider.h"
#include "PlaneCollisionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories=(Variable, Sockets, Tags, ComponentTick, ComponentReplication, Activation, Cooking, Events, AssetUserData, Replication, Navigation))
class UPlaneCollisionComponent : public UCollisionComponent
{
	GENERATED_BODY()
public:
	UPlaneCollisionComponent() {
		PlaneCollider = FPlaneCollider();
		PrimaryComponentTick.bCanEverTick = true;
	}

	virtual const FPlaneCollider& GetCollider() const override {
		return PlaneCollider;
	}

	virtual void DrawDebugCollider() const override;

	virtual bool TestCollision(const UCollisionComponent* Other, FCollisionHit& OutCollision) const override
	{
		return PlaneCollider.TestCollision(GetOwner()->GetActorTransform(), Other->GetCollider(), Other->GetOwner()->GetActorTransform(), OutCollision);
	}

	virtual bool TestCollision(ANoiseFloor* Other, FCollisionHit& OutCollision) const override
	{
		return PlaneCollider.TestCollision(GetOwner()->GetActorTransform(), Other, OutCollision);
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlaneCollider PlaneCollider;
	
};
