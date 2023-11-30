#pragma once

#include "CoreMinimal.h"
#include "Utils/EasingType.h"
#include "Utils/EasingUtils.h"
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

	void ResetValues(const EEasingType NewEasingType = GetRandomEasingType())
	{
		T = 0;
		EasingValue = 0;
		EasingType = NewEasingType;
	}

	float GetAndUpdateInterpolationValue(const float DeltaTime)
	{
		T += DeltaTime / MoveDuration;
		T = FMath::Clamp(T, 0.f, 1.f);
		EasingValue = UEasingUtils::GetEaseValue(EasingType, T);
		return EasingValue;
	}
	

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
	UPROPERTY(EditAnywhere, Category = "Interpolation")
	TEnumAsByte<EEasingType> EasingType = EEasingType::Linear;
	UPROPERTY(EditAnywhere, Category = "Interpolation")
	bool bUseRandomEasingType = false;
	UPROPERTY(VisibleAnywhere, Category = "Interpolation")
	float MoveDuration = 5.0f;
	
private:
	UPROPERTY()
	AMathActor* MathActor;
	UPROPERTY()
	UMathMovementComponent* MovementComponent;
	
};
