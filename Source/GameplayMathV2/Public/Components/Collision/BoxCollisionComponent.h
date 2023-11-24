#pragma once
#include "CollisionComponent.h"
#include "Structs/Colliders/BoxCollider.h"
#include "BoxCollisionComponent.generated.h"

class ANoiseFloor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories=(Variable, Sockets, Tags, ComponentTick, ComponentReplication, Activation, Cooking, Events, AssetUserData, Replication, Navigation))
class UBoxCollisionComponent : public UCollisionComponent
{
	GENERATED_BODY()
public:
	UBoxCollisionComponent() {
		BoxCollider = FBoxCollider();
		PrimaryComponentTick.bCanEverTick = true;
	}

	virtual const FBoxCollider& GetCollider() const override {
		return BoxCollider;
	}

	virtual void DrawDebugCollider() const override;

	virtual bool TestCollision(const UCollisionComponent* Other, FCollisionHit& OutCollision) const override
	{
		return BoxCollider.TestCollision(GetOwner()->GetActorTransform(), Other->GetCollider(), Other->GetOwner()->GetActorTransform(), OutCollision);
	}

	virtual bool TestCollision(ANoiseFloor* Other, FCollisionHit& OutCollision) const override
	{
		return BoxCollider.TestCollision(GetOwner()->GetActorTransform(), Other, OutCollision);
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FBoxCollider BoxCollider;
	
};
