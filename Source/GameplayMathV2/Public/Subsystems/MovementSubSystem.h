#pragma once

#include "CoreMinimal.h"
#include "Utils/EasingType.h"
#include "MovementSubsystem.generated.h"

class UMathMovementComponent;
class AMathActor;

UCLASS()
class GAMEPLAYMATHV2_API UMovementSubSystem : public UTickableWorldSubsystem
{
public:
	GENERATED_BODY()

	virtual void Tick(float DeltaTime) override;
	
	void Register(UMathMovementComponent* InComponent, EEasingType& EasingType);
	void Register(UMathMovementComponent* InComponent);

	void Unregister(UMathMovementComponent* InComponent);

	void UpdateMoveData(float DeltaTime);
	
	void SetDrawDebug(const bool bNewDrawDebug) { bDrawDebug = bNewDrawDebug; }

	virtual TStatId GetStatId() const override;

private:
	UPROPERTY()
	TArray<UMathMovementComponent*> MovementComponents;

	UPROPERTY()
	bool bDrawDebug;

	EEasingType LatestEasingType = EEasingType::Linear;
	
};