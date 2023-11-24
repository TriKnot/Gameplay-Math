#pragma once
#include "CollisionComponent.h"
#include "Structs/Colliders/SphereCollider.h"
#include "SphereCollisionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories=(Variable, Sockets, Tags, ComponentTick, ComponentReplication, Activation, Cooking, Events, AssetUserData, Replication, Navigation))
class USphereCollisionComponent : public UCollisionComponent
{
	GENERATED_BODY()
public:
	USphereCollisionComponent() {
		SphereCollider = FSphereCollider();
		PrimaryComponentTick.bCanEverTick = true;
	}

	virtual const FSphereCollider& GetCollider() const override {
		return SphereCollider;
	}

	virtual void DrawDebugCollider() const override;
	
	virtual bool TestCollision(const UCollisionComponent* Other, FCollisionHit& OutCollision) const override
	{
		return SphereCollider.TestCollision(GetOwner()->GetActorTransform(), Other->GetCollider(), Other->GetOwner()->GetActorTransform(), OutCollision);
	}

	virtual bool TestCollision(ANoiseFloor* Other, FCollisionHit& OutCollision) const override
	{
		return SphereCollider.TestCollision(GetOwner()->GetActorTransform(), Other, OutCollision);
	}
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSphereCollider SphereCollider;
	
};
