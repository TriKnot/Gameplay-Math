#pragma once
#include "CoreMinimal.h"
#include "Collider.h"
#include "PlaneCollider.generated.h"

USTRUCT(BlueprintType)
struct FPlaneCollider : public FCollider
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Extent;

	
	virtual EColliderType GetType() const override
	{
		return EColliderType::Plane;
	}

};

