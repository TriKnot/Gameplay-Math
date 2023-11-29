#pragma once

#include "CoreMinimal.h"
#include "ContextSubsystem.generated.h"

class UContextComponent;

UCLASS()
class GAMEPLAYMATHV2_API UContextSubsystem : public UWorldSubsystem
{
public:
	GENERATED_BODY()

	void Register(UContextComponent* Component) { Components.AddUnique(Component); }
	void Unregister(UContextComponent* Component) { Components.Remove(Component); }
	void UpdateActorContext(UContextComponent* Component) const;

	UContextComponent* GetClosestContextComponent(const UContextComponent* Component) const;

private:
	UPROPERTY()
	TArray<UContextComponent*> Components; 
	
};
