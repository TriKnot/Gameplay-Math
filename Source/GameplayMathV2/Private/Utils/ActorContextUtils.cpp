#include "..\..\Public\Utils\ContextUtils.h"
#include "Components/ContextComponent.h"
#include "Utils/RelativeContext.h"

int32 UContextUtils::GetActorContext(UContextComponent* Component, UContextComponent* OtherComponent)
{
	int32 Result = 0;
	const FContextParameters ActorParams = Component->GetContextParameters();
	const FContextParameters OtherActorParams = OtherComponent->GetContextParameters();

	AActor* Actor = Component->GetOwner();
	AActor* OtherActor = OtherComponent->GetOwner();
	
	if (IsInFrontOf(Actor, OtherActor))
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::InFront));
	}
	else
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::Behind));
	}

	if(IsOnRightSideOf(Actor, OtherActor))
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::ToTheRight));
	}
	else
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::ToTheLeft));
	}

	if(IsAbove(Actor, OtherActor))
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::Above));
	}
	else
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::Below));
	}

	float Distance = FVector::Dist(Actor->GetActorLocation(), OtherActor->GetActorLocation());
	
	if(Distance < ActorParams.VisionRange)
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::Close));
	}
	else
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::Far));
	}


	if(FindAngleBetween(Actor->GetActorForwardVector(), OtherActor->GetActorForwardVector()) < 90.0f)
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::FacingSame));
	}
	else
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::FacingOpposite));
	}
	
	const bool bIsInVisionRange = Distance < OtherActorParams.VisionRange; // If the other actor is within the vision range
	const FVector DirectionToOther = (Actor->GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
	const bool bIsInVisionAngle = FindAngleBetween(Actor->GetActorForwardVector(), DirectionToOther) < OtherActorParams.VisionAngle * 0.5f; // If the other actor is within the vision angle
	
	if( bIsInVisionRange && bIsInVisionAngle)
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::Seen));
	}
	else
	{
		SET_BIT(Result, static_cast<int32>(ERelativeContext::Unseen));
	}

	return Result;
	
}

bool UContextUtils::IsInFrontOf(AActor* Actor, AActor* OtherActor)
{
	return Actor->GetActorTransform().InverseTransformPosition(OtherActor->GetActorLocation()).X > 0;
}

bool UContextUtils::IsOnRightSideOf(AActor* Actor, AActor* OtherActor)
{
	return Actor->GetActorTransform().InverseTransformPosition(OtherActor->GetActorLocation()).Y > 0;
}

bool UContextUtils::IsAbove(AActor* Actor, AActor* OtherActor)
{
	return Actor->GetActorTransform().InverseTransformPosition(OtherActor->GetActorLocation()).Z > 0;
}

bool UContextUtils::IsFacingAway(AActor* Actor, AActor* OtherActor)
{
	const FVector DirectionToOther = (OtherActor->GetActorLocation() - Actor->GetActorLocation()).GetSafeNormal();
	return FVector::DotProduct(Actor->GetActorForwardVector(), DirectionToOther) < 0;
}

float UContextUtils::FindAngleBetween(const FVector& Forward, const FVector& OtherForward)
{
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, OtherForward)));
}
