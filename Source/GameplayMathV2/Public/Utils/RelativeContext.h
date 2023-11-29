#pragma once

UENUM(BlueprintType, Meta = (Bitflags))
enum class ERelativeContext
{
	InFront,
	Behind,
	ToTheRight,
	ToTheLeft,
	Above,
	Below,
	Close,
	Far,
	Seen,
	Unseen,
	FacingSame,
	FacingOpposite
};