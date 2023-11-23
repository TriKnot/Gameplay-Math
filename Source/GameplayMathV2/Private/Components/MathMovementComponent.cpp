#include "Components/MathMovementComponent.h"
#include "Actor/MathActor.h"
#include "Components/Collision/CollisionComponent.h"
#include "Subsystems/CollisionSubsystem.h"
#include "Subsystems/MovementSubSystem.h"

void UMathMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Actor = GetOwner();
	Owner = Cast<AMathActor>(Actor);
	if(!Owner)	return;
	
	// Add Velocity in a random Direction
	const FVector RandomDirection = FMath::VRand();
	AddVelocity(RandomDirection * MoveData.MoveSpeed);
	
	// Find Movement Subsystem and unregister
	if(const TObjectPtr<UMovementSubSystem> MovementSubSystem = GetWorld()->GetSubsystem<UMovementSubSystem>())
		MovementSubSystem->Register(this);

	// Find Collision Component and register
	if(Owner)
		CollisionComponent = Owner->FindComponentByClass<UCollisionComponent>();

}

void UMathMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Find Movement Subsystem and unregister
	if(const TObjectPtr<UMovementSubSystem> MovementSubSystem = GetWorld()->GetSubsystem<UMovementSubSystem>())
		MovementSubSystem->Unregister(this);

}

void UMathMovementComponent::Step(float DeltaTime)
{
	if(bUseGravity)
	{
		MoveData.Velocity.Z += Gravity * DeltaTime;
	}
	const FVector CurrentLocation = Owner->GetActorLocation();
	const FVector StepVelocity = MoveData.Velocity * DeltaTime;
	FVector TargetLocation = CurrentLocation + StepVelocity;

	const FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	const TObjectPtr<UCollisionSubsystem> CollisionSubsystem = GetWorld()->GetSubsystem<UCollisionSubsystem>();
	if(CollisionComponent && CollisionSubsystem)
	{
		FCollisionHit CollisionHit;
		const FVector RayStartLocation = CurrentLocation + CollisionComponent->GetCollider().GetBoundaryOffset(Direction);
		const FVector RayEndLocation = TargetLocation + CollisionComponent->GetCollider().GetBoundaryOffset(Direction);
		const FVector RayDirection = (RayEndLocation - RayStartLocation).GetSafeNormal();
		const float RayLength = (RayEndLocation - RayStartLocation).Size() * 1.5f;
		DrawDebugLine(GetWorld(), RayStartLocation, RayEndLocation, FColor::Red);
		if(CollisionSubsystem->RayCast(RayStartLocation, RayDirection, RayLength, CollisionHit))
		{
			TargetLocation = CollisionHit.CollisionPoint;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Raycast hit!"));
		}
	}
	
	Owner->SetActorLocation(TargetLocation);
	
	//DrawDebugLine(GetWorld(), CurrentLocation, StepVelocity * 100.0f, FColor::Red);
	DrawDebugSphere(GetWorld(), CurrentLocation, 10.0f, 12, FColor::Green);
	DrawDebugSphere(GetWorld(), TargetLocation, 10.0f, 12, FColor::Blue);
}

void UMathMovementComponent::AddVelocity(const FVector& Velocity)
{
	MoveData.Velocity += Velocity;
}

void UMathMovementComponent::StopMovement()
{
	MoveData.Velocity = FVector::ZeroVector;
}
