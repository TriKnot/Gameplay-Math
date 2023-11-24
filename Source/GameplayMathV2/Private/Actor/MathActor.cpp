
#include "Actor/MathActor.h"

// Sets default values
AMathActor::AMathActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMathActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMathActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMathActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

