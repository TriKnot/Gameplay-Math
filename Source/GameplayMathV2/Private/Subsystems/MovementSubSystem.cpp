#include "Subsystems/MovementSubsystem.h"
#include "Components/MathMovementComponent.h"

void UMovementSubSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMoveData(GetWorld()->GetDeltaSeconds());
}


void UMovementSubSystem::Register(UMathMovementComponent* InComponent, EEasingType& EasingTypeToUpdate) {
	EasingTypeToUpdate = LatestEasingType;
	LatestEasingType = GetNextEasingType(LatestEasingType);
	Register(InComponent);
}

void UMovementSubSystem::Register(UMathMovementComponent* InComponent)
{
	// Make sure we don't register the same actor twice
	for (const UMathMovementComponent* Component : MovementComponents)
	{
		if(Component == InComponent)
		{
			return;
		}
	}
	MovementComponents.Add(InComponent);
}

void UMovementSubSystem::Unregister(UMathMovementComponent* InComponent)
{
	// Find and remove MoveData
	MovementComponents.Remove(InComponent);
}

void UMovementSubSystem::UpdateMoveData(float DeltaTime)
{
	for (UMathMovementComponent* Component : MovementComponents)
	{
		// Update MoveData
		Component->Step(DeltaTime);
	}
}

TStatId UMovementSubSystem::GetStatId() const
{
	return GetStatID();
}
