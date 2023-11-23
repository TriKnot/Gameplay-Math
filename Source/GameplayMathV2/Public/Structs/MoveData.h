#pragma once

#include "CoreMinimal.h"
#include "MoveData.generated.h"

class AMathActor;
class UMathMovementComponent;

USTRUCT(BlueprintType)
struct FMoveData
{
	GENERATED_BODY()
public:
	FMoveData() = default;

	float GetTValue() const { return T; }
	AMathActor* GetMathActor() const { return MathActor; }
	UMathMovementComponent* GetMovementComponent() const { return MovementComponent; }

public:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, Category = "Interpolation")
	FVector TargetLocation;
	UPROPERTY(VisibleAnywhere, Category = "Interpolation")
	FVector StartLocation;
	UPROPERTY(EditAnywhere, Category = "Interpolation")
	float EasingValue;
	UPROPERTY(VisibleAnywhere, Category = "Interpolation")
	float T;
	
private:
	UPROPERTY()
	AMathActor* MathActor;
	UPROPERTY()
	UMathMovementComponent* MovementComponent;
	
};
