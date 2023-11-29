#include "Utils/CollisionUtility.h"
#include "Components/Collision/CollisionComponent.h"
#include "Utils/IntersectionUtility.h"


bool UCollisionUtility::PlanePlaneCollision(	const FVector& NormalA, const FVector2d BoundsA,
	                                            const FVector& NormalB, const FVector2d BoundsB, FCollisionHit& CollisionHit)
{
	// TODO: Finish Plane-Plane Intersection with bounds and check fro collisionpoint
	return UIntersectionUtility::PlanePlaneIntersection(NormalA, BoundsA, NormalB, BoundsB);
}

bool UCollisionUtility::SpherePlaneCollision(	const FVector& SphereCenter, const float SphereRadius,
	const FVector& PlaneCenter, const FVector& PlaneNormal, const FVector2d& PlaneBounds, FCollisionHit& CollisionHit)
{
	const bool bIntersects = UIntersectionUtility::SpherePlaneIntersection(SphereCenter, SphereRadius, PlaneCenter, PlaneNormal, PlaneBounds);
	
	// Check if the distance is less than or equal to the sphere radius
	if (bIntersects)
	{
		const FVector LocalSphereCenter = SphereCenter - PlaneCenter;
		const FVector LocalAdjustedPoint = LocalSphereCenter - PlaneNormal * SphereRadius;
		const FVector PointInPlane = LocalAdjustedPoint + PlaneCenter;
		const FVector CollisionNormal = (SphereCenter - PointInPlane).GetSafeNormal();
		
		CollisionHit.Depth = SphereRadius - SphereCenter.Dot(PlaneNormal) - PlaneNormal.Dot(PlaneCenter);
		CollisionHit.CollisionNormal = CollisionNormal;
		CollisionHit.CollisionPoint = PointInPlane + PlaneNormal * CollisionHit.Depth;
		return true;
	}
	
	return false;
}

bool UCollisionUtility::SphereSphereCollision(const FVector& CenterA, const float RadiusA, const FVector& CenterB,
	const float RadiusB, FCollisionHit& CollisionHit)
{
	const FVector Center = CenterA - CenterB;
	const float RadiusSum = RadiusA + RadiusB;

	if(UIntersectionUtility::SphereSphereIntersection(CenterA, RadiusA, CenterB, RadiusB))
	{
		const float Distance = FMath::Sqrt(Center.SizeSquared());
		CollisionHit.CollisionPoint = CenterB + (Center * (RadiusB / Distance));
		CollisionHit.CollisionNormal = Center.GetSafeNormal();
		CollisionHit.Depth = (RadiusSum - Distance) * 1.5;
		return true;
	}
	return false;
}

bool UCollisionUtility::RaySphereCollision(const FVector& Origin, const FVector& RayDirection, const float RayLength,
	const FVector& SphereCenter, const float SphereRadius, FCollisionHit& CollisionHit)
{
	const FVector RayToSphere = SphereCenter - Origin;

	if (RayToSphere.SizeSquared() > FMath::Square(RayLength + SphereRadius))
	{
		// Sphere is too far away from the ray
		return false;
	}

	const float Projection = RayToSphere | RayDirection;

	if(Projection < 0.f)
	{
		// Sphere is behind the ray
		return false;
	}

	const FVector ClosestPointOnRay = Origin + (RayDirection * Projection);
	const float DistanceToSphereCenter = FVector::Distance(ClosestPointOnRay, SphereCenter);

	// If the distance to the sphere center is greater than the sphere radius, there is no intersection
	if (DistanceToSphereCenter > SphereRadius)
	{
		return false;
	}

	// Calculate the distance from the closest point on the ray to the intersection point
	const float DistanceToIntersection = FMath::Sqrt(SphereRadius * SphereRadius - DistanceToSphereCenter * DistanceToSphereCenter);

	// Calculate the intersection point
	const FVector IntersectionPoint = ClosestPointOnRay - DistanceToIntersection * RayDirection;

	CollisionHit.CollisionNormal = -RayToSphere.GetSafeNormal();
	CollisionHit.CollisionPoint = IntersectionPoint;
	CollisionHit.Depth = DistanceToIntersection;
	
	return true;
}

bool UCollisionUtility::RayPlaneCollision(const FVector& RayOrigin, const FVector& RayDirection, const float RayLength,
	const FVector& PlaneCenter, const FVector& PlaneNormal, FCollisionHit& CollisionHit)
{
	const float DirectionDot = RayDirection | PlaneNormal;

	// Check if the ray is parallel to the plane
	if (DirectionDot >= 0.0f)
	{
		return false;
	}

	const float Distance = ((PlaneCenter - RayOrigin) | PlaneNormal) / DirectionDot;
							
	// Check if the plane if behind the ray
	if(Distance < 0.f)
	{
		return false;
	}
	
	if(	Distance >= RayLength)
	{
		return false;
	}
	
	const FVector IntersectionPoint = RayOrigin + Distance * RayDirection;
	CollisionHit.CollisionNormal = IntersectionPoint - PlaneCenter;
	CollisionHit.CollisionPoint = IntersectionPoint;
	CollisionHit.Depth = Distance;
	
	return true;
}

bool UCollisionUtility::RayAABBCollision(const FVector& RayOrigin, const FVector& RayDirection,
	const float RayLength, const FVector& Min, const FVector& Max, FCollisionHit& CollisionHit)
{
	float TxMin = (Min.X - RayOrigin.X) / RayDirection.X;
	float TxMax = (Max.X - RayOrigin.X) / RayDirection.X;

	if(TxMin > TxMax)	Swap(TxMin, TxMax);

	float TyMin = (Min.Y - RayOrigin.Y) / RayDirection.Y;
	float TyMax = (Max.Y - RayOrigin.Y) / RayDirection.Y;

	if(TyMin > TyMax)	Swap(TyMin, TyMax);

	if(TxMin > TyMax || TyMin > TxMax)
		return false;

	if(TyMin > TxMin)	TxMin = TyMin;
	if(TyMax < TxMax)	TxMax = TyMax;

	float TzMin = (Min.Z - RayOrigin.Z) / RayDirection.Z;
	float TzMax = (Max.Z - RayOrigin.Z) / RayDirection.Z;

	if(TzMin > TzMax)	Swap(TzMin, TzMax);

	const float TMaxFinal = FMath::Min(TxMax, FMath::Min(TyMax, TzMax));
	const float TMinFinal = FMath::Max(TxMin, FMath::Max(TyMin, TzMin));

	if(TMaxFinal >= 0 && TMinFinal <= RayLength && TMinFinal <= TMaxFinal)
	{

		// Calculate the normal based on which face is intersected
		if (TMinFinal == TxMin) {
			// Intersected with the face having minimum X-coordinate
			CollisionHit.CollisionNormal = FVector(-1, 0, 0);
		} else if (TMinFinal == TyMin) {
			// Intersected with the face having minimum Y-coordinate
			CollisionHit.CollisionNormal = FVector(0, -1, 0);
		} else if (TMinFinal == TzMin) {
			// Intersected with the face having minimum Z-coordinate
			CollisionHit.CollisionNormal = FVector(0, 0, -1);
		} else if (TMaxFinal == TxMax) {
			// Intersected with the face having maximum X-coordinate
			CollisionHit.CollisionNormal = FVector(1, 0, 0);
		} else if (TMaxFinal == TyMax) {
			// Intersected with the face having maximum Y-coordinate
			CollisionHit.CollisionNormal = FVector(0, 1, 0);
		} else if (TMaxFinal == TzMax) {
			// Intersected with the face having maximum Z-coordinate
			CollisionHit.CollisionNormal = FVector(0, 0, 1);
		}
		
		return true;
	}
	
	return false;;
}

bool UCollisionUtility::AABBAABBCollision(const FVector& MinA, const FVector& MaxA, const FVector& MinB,
                                            const FVector& MaxB, FCollisionHit& CollisionHit)
{
	// TODO Calculate the collision point
	
	return UIntersectionUtility::AABBAABBIntersection(MinA, MaxA, MinB, MaxB);
}

bool UCollisionUtility::PointAABBCollision(const FVector& Point, const FVector& Min, const FVector& Max,
	FCollisionHit& CollisionHit)
{
	if(UIntersectionUtility::PointAABBIntersection(Point, Min, Max))
	{
		CollisionHit.CollisionNormal = FVector::ZeroVector;
		CollisionHit.CollisionPoint = Point;
		return true;
	}
	
	return false;
}

bool UCollisionUtility::SphereAABBCollision(const FVector& SphereCenter, const float SphereRadius,
	const FVector& AABBCenter, const FVector& AABBExtents, FCollisionHit& CollisionHit)
{
	FVector ClosestPoint;
	if (UIntersectionUtility::SphereAABBIntersection(SphereCenter, SphereRadius, AABBCenter, AABBExtents, ClosestPoint))
	{
		const FVector CollisionPoint = ClosestPoint + AABBCenter;
		CollisionHit.CollisionPoint = CollisionPoint;
		CollisionHit.CollisionNormal = (CollisionPoint - SphereCenter).GetSafeNormal(); 
		CollisionHit.Depth = (CollisionPoint - SphereCenter).Size() - SphereRadius; 

		return true; 
	}

	return false;
}

bool UCollisionUtility::AABBPlaneCollision(const FVector& AABBLocation, const FVector& AABBExtent,
    const FTransform& PlaneTransform, const FVector& PlaneNormal, const FVector2D PlaneExtent, FCollisionHit& CollisionHit)
{
	// TODO Calculate the collision point
	return UIntersectionUtility::AABBPlaneIntersection(AABBLocation, AABBExtent, PlaneTransform, PlaneNormal, PlaneExtent);
}

bool UCollisionUtility::RayTriangle(const FVector& Origin, const FVector& Direction, const FVector& V0, const FVector& V1,
	const FVector& V2, FCollisionHit& ContactPoint)

{
	const FVector E1 = V1 - V0;
	const FVector E2 = V2 - V0; 

	const FVector P = Direction.Cross(E2);
	const float A = E1.Dot(P);

	// No intersection or infinite intersections
	if(FMath::IsNearlyEqual(A, 0.f))
		return false;

	const float F = 1.f / A;
	const FVector S = Origin - V0;
	const float U = F * S.Dot(P);

	// Outside triangle bounds on U axis.
	if(U < 0.f || U > 1.f)
		return false;

	const FVector Q = S.Cross(E1);
	const float V = F * Direction.Dot(Q);

	// Outside triangle bounds V axis (remove 'U +' to check quad instead of triangle).
	if(V < 0.f || U + V > 1.f)
		return false;

	const float T = F * E2.Dot(Q);

	ContactPoint.CollisionPoint = Origin + (Direction * T);
	ContactPoint.CollisionNormal = (V1 - V0).Cross(V2 - V0).GetSafeNormal();
	ContactPoint.Depth = T;
		
	return T >= 0.f;
}

bool UCollisionUtility::SphereTriangleCollision(const FVector& SphereCenter, const float SphereRadius,
	const FVector& V0, const FVector& V1, const FVector& V2, FCollisionHit& CollisionHit)
{
	if(UIntersectionUtility::SphereTriangleIntersection(SphereCenter, SphereRadius, V0, V1, V2))
	{
		const FVector Normal = FVector::CrossProduct(V1 - V0, V2 - V0).GetSafeNormal();
		const FVector DirectionToTriangle = SphereCenter - V0;
		const float DistanceToPlane = FMath::Abs(FVector::DotProduct(DirectionToTriangle, Normal));
		const FVector ProjectedCenter = SphereCenter - DistanceToPlane * Normal;

		// Calculate the collision point, normal, and depth
		CollisionHit.CollisionPoint = ProjectedCenter;
		CollisionHit.CollisionNormal = Normal;
		if (FVector::DotProduct(Normal, DirectionToTriangle) < 0) {
			CollisionHit.CollisionNormal *= -1;
		}
		CollisionHit.Depth = SphereRadius;
		return true;
	}

	return false;
}
