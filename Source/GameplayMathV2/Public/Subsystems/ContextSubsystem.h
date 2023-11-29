#pragma once

#include "CoreMinimal.h"
#include "ContextSubsystem.generated.h"

class UContextComponent;
class AMathActor;

UCLASS()
class GAMEPLAYMATHV2_API UContextSubsystem : public UWorldSubsystem
{
public:
	GENERATED_BODY()

	UContextSubsystem();

	void Register(UContextComponent* Component) { Components.AddUnique(Component); }
	void Unregister(UContextComponent* Component) { Components.Remove(Component); }
	void UpdateActorContext(UContextComponent* Component);

private:
	UPROPERTY()
	TArray<UContextComponent*> Components; 
	
};
