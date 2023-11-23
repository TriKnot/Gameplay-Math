#include "Subsystems/MovementSubsystem.h"
#include "Components/MathMovementComponent.h"

void UMovementSubSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMoveData(GetWorld()->GetDeltaSeconds());
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

void UMovementSubSystem::ApplyMoveData()
{
}

// void UMovementSubSystem::MoveMathActor(AMathActor* Actor) const
// {
// 	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
// 	
// 	FMoveData& MoveData = Actor->GetMoveData();
// 	const FVector CurrentLocation = FMath::Lerp(
// 		MoveData.StartLocation,
// 		MoveData.TargetLocation,
// 		MoveData.GetAndUpdateValue(DeltaSeconds));
// 	
// 	Actor->SetActorLocation(CurrentLocation);	// Move Forward
// 		
// 	if(bDrawDebug)
// 		DrawDebugLine(GetWorld(), CurrentLocation, MoveData.TargetLocation, FColor::Red);
// }

TStatId UMovementSubSystem::GetStatId() const
{
	return GetStatID();
}
