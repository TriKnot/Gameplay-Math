#include "Subsystems/ContextSubsystem.h"
#include "Components/ContextComponent.h"
#include "Utils/ContextUtils.h"


void UContextSubsystem::UpdateActorContext(UContextComponent* Component) const
{
	if (!Component) return;
	
	UContextComponent* ClosestComponent = GetClosestContextComponent(Component);
	const int32 Context = UContextUtils::GetActorContext(Component, ClosestComponent);
	Component->SetContext(Context);
}

UContextComponent* UContextSubsystem::GetClosestContextComponent(const UContextComponent* Component) const
{
	const FVector Location = Component->GetOwner()->GetActorLocation();
	UContextComponent* ClosestComponent = nullptr;
	float ClosestDistance = FLT_MAX;
	for (UContextComponent* OtherComponent : Components)
	{
		if( OtherComponent == Component) continue;
		FVector OtherLocation = OtherComponent->GetOwner()->GetActorLocation();
		if(FVector::Dist(Location, OtherLocation) < ClosestDistance)
		{
			ClosestDistance = FVector::Dist(Location, OtherLocation);
			ClosestComponent = OtherComponent;
		}
	}
	return ClosestComponent;
}

