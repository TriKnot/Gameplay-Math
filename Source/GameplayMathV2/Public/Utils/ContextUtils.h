#pragma once

#define SET_BIT(Bitmask, Bit) (Bitmask |= 1 << static_cast<int32>(Bit))

#include "CoreMinimal.h"
#include "ContextUtils.generated.h"

class UContextComponent;

UCLASS()
class GAMEPLAYMATHV2_API UContextUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Context")
	static int32 GetActorContext(UContextComponent* Actor, UContextComponent* OtherActor);
	
public:
	UFUNCTION(BlueprintCallable, Category = "Context")
	static bool IsInFrontOf(AActor* Actor, AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Context")
	static bool IsOnRightSideOf(AActor* Actor, AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Context")
	static bool IsAbove(AActor* Actor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Context")
	static bool IsFacingAway(AActor* Actor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Context")
	static float FindAngleBetween(const FVector& Forward, const FVector& OtherForward);

};
