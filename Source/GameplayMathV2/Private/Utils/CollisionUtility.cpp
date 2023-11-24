#include "Utils/CollisionUtility.h"
#include "Structs/Colliders/SphereCollider.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Collision/CollisionComponent.h"


bool UCollisionUtility::PlanePlaneCollision(	const FVector& NormalA, const FVector2d BoundsA,
	                                            const FVector& NormalB, const FVector2d BoundsB, FCollisionHit& CollisionHit)
{
	const FVector Direction = NormalA ^ NormalB; // Cross Product

	// Check if the planes are parallel
	if (Direction.IsNearlyZero())
	{
		return false;
	}

	// TODO - Implement Plane-Plane Intersection with bounds

	return true;
}

bool UCollisionUtility::SpherePlaneCollision(	const FVector& SphereCenter, const float SphereRadius,
	const FVector& PlaneCenter, const FVector& PlaneNormal, const FVector2d& PlaneBounds, FCollisionHit& CollisionHit)
{
	const double Dist = SphereCenter.Dot(PlaneNormal) - PlaneNormal.Dot(PlaneCenter);
	
	// Check if the distance is less than or equal to the sphere radius
	if (FMath::Abs(Dist) <= SphereRadius)
	{
		const FVector LocalSphereCenter = SphereCenter - PlaneCenter;
		const FVector LocalAdjustedPoint = LocalSphereCenter - PlaneNormal * SphereRadius;
		const FVector PointInPlane = LocalAdjustedPoint + PlaneCenter;

		const FVector CollisionNormal = (SphereCenter - PointInPlane).GetSafeNormal();
		
		if(PlaneBounds == FVector2d::ZeroVector)
		{
			CollisionHit.Depth = SphereRadius - Dist;
			CollisionHit.CollisionNormal = CollisionNormal;
			CollisionHit.CollisionPoint = PointInPlane + PlaneNormal * CollisionHit.Depth;
			return true;
		}

		// Check if the local point is within the local bounds
		const FVector2D LocalPointInPlane2D(LocalAdjustedPoint.Y, LocalAdjustedPoint.Z);
		const FVector2D LocalPlaneMinBounds(-PlaneBounds.X * 0.5f, -PlaneBounds.Y * 0.5f);
		const FVector2D LocalPlaneMaxBounds(PlaneBounds.X * 0.5f, PlaneBounds.Y * 0.5f);

		if (LocalPointInPlane2D.X >= LocalPlaneMinBounds.X && LocalPointInPlane2D.X <= LocalPlaneMaxBounds.X &&
			LocalPointInPlane2D.Y >= LocalPlaneMinBounds.Y && LocalPointInPlane2D.Y <= LocalPlaneMaxBounds.Y)
		{
			// Collision point is within the bounds of the plane
			CollisionHit.Depth = SphereRadius - Dist;
			CollisionHit.CollisionNormal = CollisionNormal;
			CollisionHit.CollisionPoint = PointInPlane + PlaneNormal * CollisionHit.Depth;
			return true;
		}
	}
	
	return false;
}

bool UCollisionUtility::SphereSphereCollision(const FVector& CenterA, const float RadiusA, const FVector& CenterB,
	const float RadiusB, FCollisionHit& CollisionHit)
{
	const FVector Center = CenterA - CenterB;
	const float RadiusSum = RadiusA + RadiusB;

	if(Center.SizeSquared() <= FMath::Square(RadiusSum))
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

	const FVector IntersectionPoint = RayOrigin + Distance * RayDirection;

	if(	Distance >= RayLength)
	{
		return false;
	}
	
	CollisionHit.CollisionNormal = IntersectionPoint - PlaneCenter;
	
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

bool UCollisionUtility::AABBCollision(const FVector& MinA, const FVector& MaxA, const FVector& MinB,
                                            const FVector& MaxB, FCollisionHit& CollisionHit)
{
	if(MinA.X > MaxB.X || MinB.X > MaxA.X)
		return false;
	if(MinA.Y > MaxB.Y || MinB.Y > MaxA.Y)
		return false;
	if(MinA.Z > MaxB.Z || MinB.Z > MaxA.Z)
		return false;

	// TODO Calculate the collision point
	
	return true;
}

bool UCollisionUtility::PointAABBCollision(const FVector& Point, const FVector& Min, const FVector& Max,
	FCollisionHit& CollisionHit)
{
	if(Point.X > Max.X || Min.X > Point.X)
		return false;
	if(Point.Y > Max.Y || Min.Y > Point.Y)
		return false;
	if(Point.Z > Max.Z || Min.Z > Point.Z)
		return false;

	CollisionHit.CollisionNormal = FVector::ZeroVector;
	
	return true;
}

bool UCollisionUtility::SphereAABBCollision(const FVector& SphereCenter, const float SphereRadius,
	const FVector& AABBCenter, const FVector& AABBExtents, FCollisionHit& CollisionHit)
{
	const FVector LocalSphereCenter = SphereCenter - AABBCenter;
	FVector ClosestPoint;

	for (int32 i = 0; i < 3; ++i)
	{
		const float component = LocalSphereCenter[i];

		if (component < -AABBExtents[i])
		{
			ClosestPoint[i] = -AABBExtents[i];
		}
		else if (component > AABBExtents[i])
		{
			ClosestPoint[i] = AABBExtents[i];
		}
		else
		{
			ClosestPoint[i] = component;
		}
	}

	const float SquaredDistToClosest = (ClosestPoint - LocalSphereCenter).SizeSquared();

	if (SquaredDistToClosest <= (SphereRadius * SphereRadius))
	{
		// Calculate the collision point in world space
		const FVector CollisionPoint = ClosestPoint + AABBCenter;
		CollisionHit.CollisionPoint = CollisionPoint; // Point of collision
		CollisionHit.CollisionNormal = (CollisionPoint - SphereCenter).GetSafeNormal(); // Normal of the collision (from sphere center to collision point)
		CollisionHit.Depth = (CollisionPoint - SphereCenter).Size() - SphereRadius; // Depth between the surfaces

		return true; // Intersection detected
	}

	return false; // No intersection
}

bool UCollisionUtility::AABBPlaneIntersection(const FVector& AABBLocation, const FVector& AABBExtent,
    const FTransform& PlaneTransform, const FVector& PlaneNormal, const FVector2D PlaneExtent, FCollisionHit& CollisionHit)
{
	// Calculate the AABB bounds from its center and extents
	const FVector AABBMin = AABBLocation - AABBExtent;
	const FVector AABBMax = AABBLocation + AABBExtent;

	// Convert AABB to center-extents representation
	const FVector Center = (AABBMax + AABBMin) * 0.5f; // Compute AABB center
	const FVector Extents = AABBMax - Center; // Compute positive extents

	// Calculate the projection interval radius of AABB onto the plane's normal
	const float Radius = Extents.X * FMath::Abs(PlaneNormal.X) +
				   Extents.Y * FMath::Abs(PlaneNormal.Y) +
				   Extents.Z * FMath::Abs(PlaneNormal.Z);

	// Compute the distance of the box center from the plane
	const float Distance = FVector::DotProduct(PlaneNormal, Center - PlaneTransform.GetTranslation());

	// Check for intersection based on distance and radius
	return FMath::Abs(Distance) <= Radius;
}


bool UCollisionUtility::IsOnScreen(const APlayerController* PlayerController, const UCollisionComponent* Component,
	const float ScreenSpaceCompare)
{
	const FCollider Collider = Component->GetCollider();
	
	// If Shere
	if(Collider.GetType() == EColliderType::Sphere)
	{
		const FSphereCollider SphereCollider = static_cast<const FSphereCollider&>(Collider);
		return IsSphereOnScreen(Component->GetOwner()->GetActorLocation(), SphereCollider.Radius, PlayerController);
	}
	// If Plane
	// if(Collider.GetType() == EColliderType::Plane)
	// {
	// 	const FPlaneCollider PlaneCollider = static_cast<const FPlaneCollider&>(Collider);
	// 	return IsPlaneOnScreen(Component->GetOwner()->GetActorLocation(), PlaneCollider.GetNormal(), PlaneCollider.Extent, PlayerController);
	// }
	return false;
}

bool UCollisionUtility::IsPointOnScreen(const APlayerController* PlayerController, const FVector& WorldPosition,
                                           const float ScreenSpaceCompare)
{
	FVector2d ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(PlayerController, WorldPosition, ScreenPosition);

	int32 ViewportSizeX, ViewportSizeY;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	return (ScreenPosition.X > ScreenSpaceCompare && ScreenPosition.X < ViewportSizeX - ScreenSpaceCompare) &&
		(ScreenPosition.Y > ScreenSpaceCompare && ScreenPosition.Y < ViewportSizeY - ScreenSpaceCompare);
}

bool UCollisionUtility::IsAABBOnScreen(const FVector& Min, const FVector& Max, const APlayerController* PlayerController,
	const float ScreenCompare)
{
	return IsPointOnScreen(PlayerController, Min, ScreenCompare)
		&& IsPointOnScreen(PlayerController, Max, ScreenCompare);
}

bool UCollisionUtility::IsSphereOnScreen(const FVector& Center, const float Radius,
	const APlayerController* PlayerController, const float ScreenCompare)
{
	// TODO add radius to the check
	return IsPointOnScreen(PlayerController, Center, ScreenCompare );	
}

bool UCollisionUtility::IsPlaneOnScreen(const FVector& Center, const FVector& Normal, const FVector2d Bounds,
	const APlayerController* PlayerController, const float ScreenCompare)
{
	// Check if center is on screen first
	if(IsPointOnScreen(PlayerController, Center, ScreenCompare))
	{
		return true;
	}

	// TODO - Implement the rest of Plane On Screen Test
	return false;
}

bool UCollisionUtility::RayTriangle(const FVector& Origin, const FVector& Direction, const FVector& V0, const FVector& V1,
	const FVector& V2, FCollisionHit& ContactPoint)

{
	const FVector E1 = V1 - V0;
	const FVector E2 = V2 - V0; 

	const auto P = Direction.Cross(E2);
	const auto A = E1.Dot(P);

	// No intersection or infinite intersections
	if(FMath::IsNearlyEqual(A, 0.f))
		return false;

	const auto F = 1.f / A;
	const auto S = Origin - V0;
	const auto U = F * S.Dot(P);

	// Outside triangle bounds on U axis.
	if(U < 0.f || U > 1.f)
		return false;

	const auto Q = S.Cross(E1);
	const auto V = F * Direction.Dot(Q);

	// Outside triangle bounds V axis (remove 'U +' to check quad instead of triangle).
	if(V < 0.f || U + V > 1.f)
		return false;

	const auto T = F * E2.Dot(Q);

	ContactPoint.CollisionPoint = Origin + (Direction * T);
	ContactPoint.CollisionNormal = (V1 - V0).Cross(V2 - V0).GetSafeNormal();
	ContactPoint.Depth = T;
		
	return T >= 0.f;
}

bool UCollisionUtility::SphereTriangleIntersection(const FVector& SphereCenter, const float SphereRadius,
	const FVector& V0, const FVector& V1, const FVector& V2, FCollisionHit& CollisionHit)
{
	// Calculate the normal of the triangle
	const FVector Normal = FVector::CrossProduct(V1 - V0, V2 - V0).GetSafeNormal();

	// Calculate the direction from the sphere center to the triangle
	const FVector DirectionToTriangle = SphereCenter - V0;

	// Calculate the distance from the sphere center to the triangle plane
	const float DistanceToPlane = FMath::Abs(FVector::DotProduct(DirectionToTriangle, Normal));
	
	// Check if sphere center is outside the triangle plane
	if (DistanceToPlane > SphereRadius ) {
		return false;
	}
	
	// Calculate the projection of the sphere center onto the triangle plane
	const FVector ProjectedCenter = SphereCenter - DistanceToPlane * Normal;

	// Check if the projected point is inside the triangle // TODO - Check if this is correct and optimize -> Feels very hacky
	if (!IsPointInsideTriangle(ProjectedCenter, V0, V1, V2)) {
		const FVector ProjectedCenterToV0 = V0 - ProjectedCenter;
		if(IsPointInsideTriangle(ProjectedCenterToV0, V0, V1, V2))
		{
			const FVector ProjectedCenterToV1 = V1 - ProjectedCenter;
			if(IsPointInsideTriangle(ProjectedCenterToV1, V0, V1, V2))
			{
				const FVector ProjectedCenterToV2 = V2 - ProjectedCenter;
				if(IsPointInsideTriangle(ProjectedCenterToV2, V0, V1, V2))
				{
					return false;
				}
			}
		}
	}

	// Calculate the collision point, normal, and depth
	CollisionHit.CollisionPoint = ProjectedCenter;
	CollisionHit.CollisionNormal = Normal;
	if (FVector::DotProduct(Normal, DirectionToTriangle) < 0) {
		CollisionHit.CollisionNormal *= -1;
	}
	CollisionHit.Depth = SphereRadius;

	return true;
}

bool UCollisionUtility::IsPointInsideTriangle(const FVector& Point, const FVector& V0, const FVector& V1, const FVector& V2)
{
	// Calculate vectors
	const FVector Edge0 = V1 - V0;
	const FVector Edge1 = V2 - V1;
	const FVector Edge2 = V0 - V2;

	// Calculate normals
	const FVector Normal0 = FVector::CrossProduct(Edge0, Point - V0);
	const FVector Normal1 = FVector::CrossProduct(Edge1, Point - V1);
	const FVector Normal2 = FVector::CrossProduct(Edge2, Point - V2);

	// Check if the point is on the same side of all edges' normals
	return FVector::DotProduct(Normal0, Normal1) >= 0 &&
		   FVector::DotProduct(Normal1, Normal2) >= 0 &&
		   FVector::DotProduct(Normal2, Normal0) >= 0;
}