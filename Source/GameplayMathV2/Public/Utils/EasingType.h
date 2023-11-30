#pragma once

UENUM(BlueprintType)
enum EEasingType
{
	Linear,
	SineIn,
	SineOut,
	SineInOut,
	QuadIn,
	QuadOut,
	QuadInOut,
	CubeIn,
	CubeOut,
	CubeInOut,
	QuartIn,
	QuartOut,
	QuartInOut,
	QuintIn,
	QuintOut,
	QuintInOut,
	ExponentialIn,
	ExponentialOut,
	ExponentialInOut,
	CircularIn,
	CircularOut,
	CircularInOut,
	BackBounceIn,
	BackBounceOut,
	BackBounceInOut,
	ElasticIn,
	ElasticOut,
	ElasticInOut,
	BounceIn,
	BounceOut,
	BounceInOut
};

inline EEasingType MaxEasingType = EEasingType::BounceInOut;

static EEasingType& operator++(EEasingType& EasingType)
{
	EasingType = static_cast<EEasingType>((static_cast<int>(EasingType) + 1) % static_cast<int>(MaxEasingType));
	return EasingType;
}

static EEasingType GetRandomEasingType()
{
	return static_cast<EEasingType>(FMath::RandRange(0, static_cast<int>(MaxEasingType)));
}

static EEasingType GetNextEasingType(EEasingType CurrentEasingType)
{
	return static_cast<EEasingType>((static_cast<int>(CurrentEasingType) + 1) % static_cast<int>(MaxEasingType));
}

