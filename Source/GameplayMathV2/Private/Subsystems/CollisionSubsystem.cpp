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
				// FVector CollisionPointVector = Actor->GetActorLocation() + CollisionPoint.Normal * CollisionPoint.Depth * 0.5f;
				// DrawDebugSphere(GetWorld(), CollisionPointVector , 10.0f, 3, FColor::Green, false, 15.0f, 0, 1.0f);
				Component->HandleCollisions();
			}
		}
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
