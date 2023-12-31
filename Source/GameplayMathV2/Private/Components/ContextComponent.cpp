﻿#include "Components/ContextComponent.h"

#include "Actor/MathActor.h"
#include "Subsystems/ContextSubsystem.h"

UContextComponent::UContextComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetContextParameters(FContextParameters(FMath::RandRange(100.0f, 1000.0f), FMath::RandRange(45.0f, 360.0f), true));
}

void UContextComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FVector Location = GetOwner()->GetActorLocation();
	const FVector Forward = GetOwner()->GetActorForwardVector();
	
	// // Draw lines for each axis of the actors transform
	const FVector Right = GetOwner()->GetActorRightVector();
	const FVector Cross = FVector::CrossProduct(Forward, Forward);
	DrawDebugLine(GetWorld(), Location, Location + Forward * 100.0f, FColor::Red);
	DrawDebugLine(GetWorld(), Location, Location + Right * 100.0f, FColor::Green);
	DrawDebugLine(GetWorld(), Location, Location + Cross * 100.0f, FColor::Blue);

	if(!bParseContext) return;
	
	// Draw cone for vision
	int32 NumSegments = FMath::CeilToInt(ContextParameters.VisionAngle / 10.0f);	// 10 degrees per segment
	NumSegments = NumSegments % 2 == 0 ? NumSegments : NumSegments + 1;				// Make sure it's even
	DrawDebugCone(
		GetWorld(),
		Location + FVector::UpVector * 10.0f,
		Forward,
		ContextParameters.VisionRange,
		FMath::DegreesToRadians(ContextParameters.VisionAngle * 0.5f),
		0.f,
		NumSegments,
		FColor::Green
	);
	
	ContextUpdateFrequencyFrameCount++;
	// Update context if actor has moved
	if (ContextUpdateFrequencyFrameCount >= ContextUpdateFrequency )
	{
		ContextSubsystem->UpdateActorContext(this);
		ContextUpdateFrequencyFrameCount = 0;
		ParseContext();
	}
}


void UContextComponent::BeginPlay()
{
	Super::BeginPlay();
	// Find ActorRelation SubSystem and call function to add this actor to the array
	ContextSubsystem = GetWorld()->GetSubsystem<UContextSubsystem>();
	ContextSubsystem->Register(this);
	AActor* ActorOwner = GetOwner();
	if(AMathActor* MathActor = Cast<AMathActor>(ActorOwner); MathActor != nullptr)
	{
		Owner = MathActor;
	}
}

void UContextComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ContextSubsystem->Unregister(this);
}

void UContextComponent::ParseContext() const
{
	FString ContextString = "This is: " + FString::FromInt(ContextBitmask) + " - ";
	
	// Front/Back
	if((ContextBitmask & (1 << 0)) != 0)	
	{
		ContextString += "Behind, ";
	}
	else
	{
		ContextString += "In front, ";
	}

	// Right/Left
	if((ContextBitmask & (1 << 2)) != 0)	
	{
		ContextString += "To the left, ";
	}
	else							
	{
		ContextString += "To the right, ";
	}

	// Up/Down
	if((ContextBitmask & (1 << 4)) != 0)	// Below
	{
		ContextString += "Below, ";
	}
	else							// Above
	{
		ContextString += "Above, ";
	}

	// Close/Far
	if((ContextBitmask & (1 << 6)) != 0)	// Close
	{
		ContextString += "Close, ";
	}
	else							// Far
	{
		ContextString += "Far, ";
	}

	// Seen/Unseen
	if((ContextBitmask & (1 << 8)) != 0)	// Seen
	{
		ContextString += "Seen, ";
		//SetMeshColor(FLinearColor::Red);
	}
	else							// Unseen
	{
		ContextString += "Unseen, ";
		//SetMeshColor(FLinearColor::Green);
	}

	// Facing Same/Opposite
	if((ContextBitmask & (1 << 10)) != 0)	// Facing Same
	{
		ContextString += "Facing Same, ";
	}
	else							// Facing Opposite
	{
		ContextString += "Facing Opposite, ";
	}
	ContextString += "From the perspective of Closest Other Actor";
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ContextString);
	
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation() , 50.0f, 10, FColor::Yellow);
	
}
