#pragma once

UENUM(BlueprintType, Meta = (Bitflags))
enum class ERelativeContext
{
	Behind,
	InFront,
	ToTheLeft,
	ToTheRight,
	Above,
	Below,
	Close,
	Far,
	Seen,
	Unseen,
	FacingSame,
	FacingOpposite
};