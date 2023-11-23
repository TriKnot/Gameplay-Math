#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/MoveData.h"
#include "MathMovementComponent.generated.h"

class UCollisionSubsystem;
class UCollisionComponent;
class AMathActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories=(Variable, Sockets, Tags, ComponentTick, ComponentReplication, Activation, Cooking, Events, AssetUserData, Replication, Navigation))
class UMathMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Step(float DeltaTime);

	// Velocity
	FVector GetVelocity() const { return MoveData.Velocity; }
	void SetVelocity(const FVector& NewVelocity) { MoveData.Velocity = NewVelocity; }
	void AddVelocity(const FVector& Velocity);
	void StopMovement();
	
	
	AMathActor* GetMathOwner() const { return Owner; }
	
	void SetMoveData(const FMoveData& NewMoveData) { MoveData = NewMoveData; }
	FMoveData& GetMoveData() { return MoveData; }

private:
	UPROPERTY(VisibleAnywhere, meta=( AllowPrivateAccess = "true" ))
	AMathActor* Owner;
	
	UPROPERTY(EditAnywhere, Category= "Movement", meta=( AllowPrivateAccess = "true" ))
	FMoveData MoveData;

	UPROPERTY(EditAnywhere, Category= "Movement", meta=( AllowPrivateAccess = "true" ))
	bool bUseGravity = true;

	UPROPERTY(EditAnywhere, Category= "Movement", meta=( AllowPrivateAccess = "true" ))
	bool bSweep = true;

	UPROPERTY(EditAnywhere, Category= "Movement", meta=( AllowPrivateAccess = "true" ))
	int32 MaxSweepStepPerSecond = 1000;
	UPROPERTY(EditAnywhere, Category= "Movement", meta=( AllowPrivateAccess = "true" ))
	int32 MinSweepStepPerFrame = 10;

	UPROPERTY()
	UCollisionComponent* CollisionComponent;
	UPROPERTY()
	TObjectPtr<UCollisionSubsystem> CollisionSubsystem;


	inline static float Gravity = -9.81f * 100.0f;
};
