#pragma once

#include "CoreMinimal.h"
#include "ContextParameters.generated.h"

USTRUCT(BlueprintType)
struct FContextParameters
{
	GENERATED_BODY()
public:
	FContextParameters() = default;
	
	FContextParameters(const float VisionRange, const float VisionAngle, const bool bDrawDebug)
		: VisionRange(VisionRange)
		, VisionAngle(VisionAngle)
		, bDrawDebug(bDrawDebug)
	{}


public:
	UPROPERTY(EditAnywhere, Category = "Math")
	float VisionRange = 100.f;
	UPROPERTY(EditAnywhere, Category = "Math")
	float VisionAngle = 45.f;
	UPROPERTY()
	bool bDrawDebug = false;
};
