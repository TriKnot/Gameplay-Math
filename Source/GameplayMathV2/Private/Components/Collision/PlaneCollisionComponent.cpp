#include "Components/Collision/PlaneCollisionComponent.h"

void UPlaneCollisionComponent::DrawDebugCollider() const
{
	Super::DrawDebugCollider();
	if(!bDrawDebugCollider) return;

	FVector2d VisibleBounds = PlaneCollider.Extent;
	if(PlaneCollider.Extent == FVector2d::ZeroVector)
	{
		VisibleBounds = FVector2d(1000000.0f, 1000000.0f);
	}
	
	const FVector HalfRightVector = GetOwner()->GetActorTransform().GetRotation().GetRightVector() * VisibleBounds.X * 0.5f;
	const FVector HalfUpVector = GetOwner()->GetActorTransform().GetRotation().GetUpVector() * VisibleBounds.Y * 0.5f;

	const FVector Location = GetOwner()->GetActorLocation();
	const FVector UpperRight = Location + HalfRightVector + HalfUpVector;
	const FVector LowerRight = Location + HalfRightVector - HalfUpVector;
	const FVector UpperLeft = Location - HalfRightVector + HalfUpVector;
	const FVector LowerLeft = Location - HalfRightVector - HalfUpVector;

	const FColor Color = BHasCollided() ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(),UpperRight,UpperLeft, Color);
	DrawDebugLine(GetWorld(),UpperLeft,LowerLeft, Color);
	DrawDebugLine(GetWorld(),LowerLeft,LowerRight, Color);
	DrawDebugLine(GetWorld(),LowerRight,UpperRight, Color);
	DrawDebugLine(GetWorld(), UpperRight, LowerLeft, Color);
	DrawDebugLine(GetWorld(), UpperLeft, LowerRight, Color);

	DrawDebugLine(GetWorld(), Location, Location + GetOwner()->GetActorTransform().GetRotation().GetForwardVector() * 100.0f, FColor::Blue);

}
