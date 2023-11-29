#include "Subsystems/ContextSubsystem.h"
#include "Components/ContextComponent.h"
#include "Utils/ContextUtils.h"


UContextSubsystem::UContextSubsystem()
{
}

void UContextSubsystem::UpdateActorContext(UContextComponent* Component)
{
	if (!Component) return;
	
	// Loop through all actors in the world and update the context of the math actor
	for (UContextComponent* OtherComponent : Components)
	{
		UContextComponent* ContextActor = Cast<UContextComponent>(OtherComponent);
		if(ContextActor == nullptr || ContextActor == Component) continue;
		
		const int32 Context = UContextUtils::GetActorContext(Component, ContextActor);
		Component->SetContext(Context);
	}
}

