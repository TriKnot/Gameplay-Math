﻿#pragma once

UENUM(BlueprintType)
enum EEasingType
{
	None,
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

static EEasingType& operator++(EEasingType& EasingType)
{
	EasingType = static_cast<EEasingType>((static_cast<int>(EasingType) + 1) % static_cast<int>(EEasingType::BounceInOut));
	return EasingType;
}

static EEasingType GetRandomEasingType()
{
	return static_cast<EEasingType>(FMath::RandRange(0, static_cast<int>(EEasingType::BounceInOut)));
}
