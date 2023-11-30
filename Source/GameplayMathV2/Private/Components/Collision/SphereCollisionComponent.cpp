#include "Components/Collision/SphereCollisionComponent.h"

void USphereCollisionComponent::DrawDebugCollider() const
{
	Super::DrawDebugCollider();
	if(!bDrawDebugCollider) return;

	const FColor Color = BHasCollided() ? FColor::Red : FColor::Green;
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), SphereCollider.Radius, 16, Color, false, 0.0f, 0, 1.0f);
}
