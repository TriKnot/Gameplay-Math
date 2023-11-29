#include "Components/MathInterpolationMovementComponent.h"

#include "Subsystems/MovementSubSystem.h"

void UMathInterpolationMovementComponent::BeginPlay()
{
	// Find Movement Subsystem and unregister
	if(const TObjectPtr<UMovementSubSystem> MovementSubSystem = GetWorld()->GetSubsystem<UMovementSubSystem>())
		MovementSubSystem->Register(this);

	MoveData.StartLocation = GetOwner()->GetActorLocation();
	MoveData.TargetLocation = MoveData.StartLocation + FVector(0.0f, 0.0f, 10000.0f);
}

void UMathInterpolationMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Find Movement Subsystem and unregister
	if(const TObjectPtr<UMovementSubSystem> MovementSubSystem = GetWorld()->GetSubsystem<UMovementSubSystem>())
		MovementSubSystem->Unregister(this);

}

void UMathInterpolationMovementComponent::Step(float DeltaTime)
{
	const FVector CurrentLocation = FMath::Lerp(MoveData.StartLocation,	MoveData.TargetLocation, MoveData.GetAndUpdateInterpolationValue(DeltaTime));
	
	GetOwner()->SetActorLocation(CurrentLocation);	// Move Forward
	
	if(MoveData.GetTValue() == 1.0f)
	{
		const FVector Temp = MoveData.StartLocation;
		MoveData.StartLocation = MoveData.TargetLocation;
		MoveData.TargetLocation = Temp;
		if(!MoveData.bUseRandomEasingType)
			MoveData.ResetValues(MoveData.EasingType);
		else
			MoveData.ResetValues();
	}
}

void UMathInterpolationMovementComponent::StopMovement()
{
	Super::StopMovement();
	MoveData.EasingValue = EEasingType::None;
}
