#include "Subsystems/CollisionSubsystem.h"
#include "Components/Collision/CollisionComponent.h"
#include "Structs/Colliders/BoxCollider.h"
#include "Structs/Colliders/PlaneCollider.h"
#include "Structs/Colliders/SphereCollider.h"
#include "Utils/CollisionUtility.h"

UCollisionSubsystem::UCollisionSubsystem()
{
}

TStatId UCollisionSubsystem::GetStatId() const
{
	return GetStatID();
}

void UCollisionSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ClearAllCollisionsFromComponents();
	
	for (UCollisionComponent* Component : CollisionComponents)		
	{
	
		for (const UCollisionComponent* OtherComponent : CollisionComponents)
		{
			if(Component == OtherComponent)
				continue;
	
	
			FCollisionHit CollisionPoint;
			if(Component->TestCollision(OtherComponent, CollisionPoint))
			{
				//Solve Collision
				Component->AddCollision(CollisionPoint);
				Component->HandleCollisions();
				// FVector CollisionPointVector = Actor->GetActorLocation() + CollisionPoint.Normal * CollisionPoint.Depth * 0.5f;
				// DrawDebugSphere(GetWorld(), CollisionPointVector , 10.0f, 3, FColor::Green, false, 15.0f, 0, 1.0f);
			}
		}
		
		
		if(NoiseFloor)
		{
			FCollisionHit CollisionPoint;
			TestAgainstFloor(Component, CollisionPoint);
			
			// TArray<FVector> TrianglePoints;
			// NoiseFloor->FindClosestTriangle(Component->GetOwner()->GetActorLocation(), TrianglePoints);
			//
			// // Draw Closest Triangle
			// DrawDebugLine(GetWorld(), TrianglePoints[0], TrianglePoints[1], FColor::Green, false, 5.0f, 0, 1.0f);
			// DrawDebugLine(GetWorld(), TrianglePoints[1], TrianglePoints[2], FColor::Green, false, 5.0f, 0, 1.0f);
			// DrawDebugLine(GetWorld(), TrianglePoints[2], TrianglePoints[0], FColor::Green, false, 5.0f, 0, 1.0f);
			//
		}

		Component->DrawDebugCollider();
	}
}

void UCollisionSubsystem::ClearAllCollisionsFromComponents()
{
	for (UCollisionComponent* CollisionComponent : CollisionComponents)
	{
		CollisionComponent->ClearCollisions();
	}
}

bool UCollisionSubsystem::RayCast(const FVector& RayOrigin, const FVector& RayDirection, const float RayLength,
	FCollisionHit& CollisionHit)
{
	DrawDebugLine(GetWorld(), RayOrigin, RayOrigin + RayDirection * RayLength, FColor::Red, false, 15.0f, 0, 1.0f);
	for (const UCollisionComponent* Component : CollisionComponents)
	{
		// Check which component type we are testing against
		switch (Component->GetCollider().GetType())
		{
		case EColliderType::Sphere:
			{
				const FSphereCollider SphereCollider = static_cast<const FSphereCollider&>(Component->GetCollider());
				return UCollisionUtility::RaySphereCollision(RayOrigin, RayDirection, RayLength, Component->GetOwner()->GetActorLocation(), SphereCollider.Radius, CollisionHit);
			}
		case EColliderType::Plane:
			{
				const FPlaneCollider PlaneCollider = static_cast<const FPlaneCollider&>(Component->GetCollider());
				return UCollisionUtility::RayPlaneCollision(RayOrigin, RayDirection, RayLength, Component->GetOwner()->GetActorLocation(), Component->GetOwner()->GetActorForwardVector(), CollisionHit);
			}
		case EColliderType::Box:
			{
				const FBoxCollider BoxCollider = static_cast<const FBoxCollider&>(Component->GetCollider());
				return UCollisionUtility::RayAABBCollision(RayOrigin, RayDirection, RayLength, Component->GetOwner()->GetActorLocation() - BoxCollider.Extent, Component->GetOwner()->GetActorLocation() + BoxCollider.Extent, CollisionHit);
			}
		default:
			break;
		}
	}

	return false;
}

bool UCollisionSubsystem::TestCollision(UCollisionComponent* Component, FCollisionHit& CollisionHit)
{
	for (const UCollisionComponent* OtherComponent : CollisionComponents)
	{
		if(Component == OtherComponent)
			continue;
		if(OtherComponent->TestCollision(Component, CollisionHit))
		{
			return true;
		}
	}
	if(	TestAgainstFloor(Component, CollisionHit))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Bounce")));
		return true;
	}
	
	return false;
}

bool UCollisionSubsystem::TestAgainstFloor(UCollisionComponent* Component, FCollisionHit& CollisionHit) const
{
	if(Component->TestCollision(NoiseFloor, CollisionHit))
	{
		//Solve Collision
		Component->AddCollision(CollisionHit);
		Component->HandleCollisions();

		// 		
		// TArray<FVector> TrianglePoints;
		// NoiseFloor->FindClosestTriangle(Component->GetOwner()->GetActorLocation(), TrianglePoints);
		//
		// // Draw Triangle Hit
		// DrawDebugLine(GetWorld(), TrianglePoints[0], TrianglePoints[1], FColor::Red, false, 15.0f, 0, 1.0f);
		// DrawDebugLine(GetWorld(), TrianglePoints[1], TrianglePoints[2], FColor::Red, false, 15.0f, 0, 1.0f);
		// DrawDebugLine(GetWorld(), TrianglePoints[2], TrianglePoints[0], FColor::Red, false, 15.0f, 0, 1.0f);
		//
		// // Draw Collision Point 
		// DrawDebugBox(GetWorld(), Component->GetOwner()->GetActorLocation(), FVector(10.0f, 10.0f, 10.0f), FColor::Blue, false, 15.0f, 0, 1.0f);
		// DrawDebugBox(GetWorld(), CollisionHit.CollisionNormal, FVector(10.0f, 10.0f, 10.0f), FColor::Blue, false, 15.0f, 0, 1.0f);
		// DrawDebugLine(GetWorld(), CollisionHit.CollisionPoint, CollisionHit.CollisionPoint + CollisionHit.CollisionNormal * CollisionHit.Depth, FColor::Red, false, 15.0f, 0, 1.0f);
		return true;
	}
	return false;
}

