#pragma once

#include "CoreMinimal.h"
#include "Structs/ContextParameters.h"
#include "ContextComponent.generated.h"


class AMathActor;
class UContextSubsystem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), hidecategories=(Variable, Sockets, Tags, ComponentTick, ComponentReplication, Activation, Cooking, Events, AssetUserData, Replication, Navigation))
class UContextComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UContextComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FContextParameters GetContextParameters() const { return ContextParameters; }
	void SetContextParameters(const FContextParameters& NewContextParameters) { ContextParameters = NewContextParameters; }

	int32 GetContext() const { return ContextBitmask; }
	void SetContext(const int32 NewContext) { ContextBitmask = NewContext; }

	void SetOwner(AMathActor* NewOwner) { Owner = NewOwner; }
	AMathActor* GetMathOwner() const { return Owner; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void ParseContext() const;
	
private:
	// Context variables
	TObjectPtr<UContextSubsystem> ActorRelationSubSystem;
	UPROPERTY()
	AMathActor* Owner;
	FVector PreviousLocation;
	FRotator PreviousRotation;
	int32 ContextBitmask;
	UPROPERTY(EditAnywhere, Category="MathActorContext", meta=( AllowPrivateAccess = "true" ))
	FContextParameters ContextParameters;
	// Frequency In frames
	UPROPERTY(EditAnywhere, Category= "Context", meta=( AllowPrivateAccess = "true" ))
	int32 ContextUpdateFrequency;
	int8 ContextUpdateFrequencyFrameCount;
};
