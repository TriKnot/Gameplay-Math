﻿#include "Components/Collision/CollisionComponent.h"

#include "Components/MathMovementComponent.h"
#include "Subsystems/CollisionSubsystem.h"

void UCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	if (UCollisionSubsystem* CollisionSubsystem = GetWorld()->GetSubsystem<UCollisionSubsystem>())
	{
		CollisionSubsystem->Register(this);
	}

	const AActor* Actor = GetOwner();
	if(!Actor)	return;
	
	// Check if we have a Collision Component
	if(UMathMovementComponent* Movement = Actor->FindComponentByClass<UMathMovementComponent>())
	{
		// Set Collision Response to Ignore
		MovementComponent = Movement;
	}
}

void UCollisionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (UCollisionSubsystem* CollisionSubsystem = GetWorld()->GetSubsystem<UCollisionSubsystem>())
	{
		CollisionSubsystem->Unregister(this);
	}
}

void UCollisionComponent::HandleCollisions()
{
	// TODO: Implement Multiple Collisions
	if(!MovementComponent) return;
	
	// Get Velocity
	const FVector Velocity = MovementComponent->GetVelocity();
	// Get Normal
	const FVector Normal = Collisions[0].CollisionNormal;

	// If inside of other collider, move this back
	if(Collisions[0].Depth > 0.0f)
	{
		GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + Normal * Collisions[0].Depth);
	}
        
	// Calculate the mirrored velocity using the reflection formula
	const FVector MirroredVelocity = Velocity - 2 * FVector::DotProduct(Velocity, Normal) * Normal;
	
	// Set the mirrored velocity to the MovementComponent
	MovementComponent->SetVelocity(MirroredVelocity);
}

void UCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DrawDebugCollider();
}

void UCollisionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UCollisionComponent::DrawDebugCollider() const
{
	if(!BHasCollided()) return;
	for (FCollisionHit Collision : Collisions)
	{
		// Draw Collision Point
		DrawDebugBox(GetWorld(), Collision.CollisionPoint, FVector(10.0f,10.0f,10.0f), GetOwner()->GetActorRotation().Quaternion(), FColor::Magenta, false, 0.0f, 0, 2.0f);
		// Draw Collision Normal
		DrawDebugLine(GetWorld(), Collision.CollisionPoint - Collision.CollisionNormal * 50.0f, Collision.CollisionPoint + Collision.CollisionNormal * 100.0f, FColor::Magenta, false, 0.0f, 0, 2.0f);

		if(Collision.A != Collision.B)
		{
			DrawDebugBox(GetWorld(), Collision.A, FVector(10.0f,10.0f,10.0f), GetOwner()->GetActorRotation().Quaternion(), FColor::Turquoise, false, 0.0f, 0, 2.0f);
			DrawDebugBox(GetWorld(), Collision.B, FVector(10.0f,10.0f,10.0f), GetOwner()->GetActorRotation().Quaternion(), FColor::Turquoise, false, 0.0f, 0, 2.0f);
			DrawDebugLine(GetWorld(), Collision.A, Collision.B, FColor::Turquoise, false, 0.0f, 0, 2.0f);
		}

	}
}
