#include "Components/Collision/BoxCollisionComponent.h"

void UBoxCollisionComponent::DrawDebugCollider() const
{
	Super::DrawDebugCollider();
	if(!bDrawDebugCollider) return;
	const FColor Color = BHasCollided() ? FColor::Red : FColor::Green;
	DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), BoxCollider.Extent, Color, false, 0.0f, 0, 1.0f);
}
