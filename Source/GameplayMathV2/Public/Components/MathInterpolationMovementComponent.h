#pragma once

#include "CoreMinimal.h"
#include "MathMovementComponent.h"

#include "MathInterpolationMovementComponent.generated.h"


class UCollisionSubsystem;
class UCollisionComponent;
class AMathActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories=(Variable, Sockets, Tags, ComponentTick, ComponentReplication, Activation, Cooking, Events, AssetUserData, Replication, Navigation))
class UMathInterpolationMovementComponent : public UMathMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Step(float DeltaTime) override;
	virtual void StopMovement() override;

};
