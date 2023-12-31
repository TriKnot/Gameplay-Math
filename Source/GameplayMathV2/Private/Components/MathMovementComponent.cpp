﻿#include "Components/MathMovementComponent.h"
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

	CollisionSubsystem = GetWorld()->GetSubsystem<UCollisionSubsystem>();

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
	const FVector TargetLocation = CurrentLocation + StepVelocity;
	const FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	
	// Sweep
	int32 MaxSweepStepCount = MaxSweepStepPerSecond * DeltaTime;
	MaxSweepStepCount = FMath::Max(MaxSweepStepCount, MinSweepStepPerFrame);
	if(bSweep && CollisionComponent && CollisionSubsystem)
	{
		const float Distance = FVector::Distance(TargetLocation, CurrentLocation);
		const float SweepStepDistance = Distance / MaxSweepStepCount;
		FCollisionHit Collision;
		for(int32 i = 0; i < MaxSweepStepCount; i++)
		{
			const FVector SweepLocation = CurrentLocation + Direction * SweepStepDistance * i;
			Owner->SetActorLocation(SweepLocation);
			if(CollisionSubsystem->TestCollision(CollisionComponent, Collision))
			{
				Owner->SetActorLocation(SweepLocation - Direction * -Collision.Depth * 2 + Direction * 0.1f);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Collision Depth: %f"), Collision.Depth));
				CollisionComponent->HandleCollision(Collision);
				break;
			}
		}
	}
	else
	{
		Owner->SetActorLocation(TargetLocation);
	}

	// Rotate towards direction
	// const FRotator TargetRotation = Direction.Rotation();
	// const FRotator CurrentRotation = Owner->GetActorRotation();
	// const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime);
	Owner->SetActorRotation(Direction.Rotation());

}

void UMathMovementComponent::AddVelocity(const FVector& Velocity)
{
	MoveData.Velocity += Velocity;
}

void UMathMovementComponent::StopMovement()
{
	MoveData.Velocity = FVector::ZeroVector;
}
