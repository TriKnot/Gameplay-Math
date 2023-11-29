#include "Utils/IntersectionUtility.h"

#include "Components/Collision/CollisionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Structs/Colliders/Collider.h"
#include "Structs/Colliders/SphereCollider.h"

bool UIntersectionUtility::PlanePlaneIntersection(const FVector& NormalA, const FVector2d BoundsA,
                                                  const FVector& NormalB, const FVector2d BoundsB)
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

bool UIntersectionUtility::SpherePlaneIntersection(const FVector& SphereCenter, const float SphereRadius,
	const FVector& PlaneCenter, const FVector& PlaneNormal, const FVector2d& PlaneBounds)
{
	const double Dist = SphereCenter.Dot(PlaneNormal) - PlaneNormal.Dot(PlaneCenter);
	
	// Check if the distance is less than or equal to the sphere radius
	if (FMath::Abs(Dist) <= SphereRadius)
	{
		const FVector LocalSphereCenter = SphereCenter - PlaneCenter;
		const FVector LocalAdjustedPoint = LocalSphereCenter - PlaneNormal * SphereRadius;
		
		if(PlaneBounds == FVector2d::ZeroVector)
		{
			return true;
		}

		// Check if the local point is within the local bounds
		const FVector2D LocalPointInPlane2D(LocalAdjustedPoint.Y, LocalAdjustedPoint.Z);
		const FVector2D LocalPlaneMinBounds(-PlaneBounds.X * 0.5f, -PlaneBounds.Y * 0.5f);
		const FVector2D LocalPlaneMaxBounds(PlaneBounds.X * 0.5f, PlaneBounds.Y * 0.5f);

		if (LocalPointInPlane2D.X >= LocalPlaneMinBounds.X && LocalPointInPlane2D.X <= LocalPlaneMaxBounds.X &&
			LocalPointInPlane2D.Y >= LocalPlaneMinBounds.Y && LocalPointInPlane2D.Y <= LocalPlaneMaxBounds.Y)
		{
			return true;
		}
	}
	
	return false;
}

bool UIntersectionUtility::SphereSphereIntersection(const FVector& CenterA, const float RadiusA, const FVector& CenterB,
	const float RadiusB)
{
	const FVector Center = CenterA - CenterB;
	const float RadiusSum = RadiusA + RadiusB;

	return Center.SizeSquared() <= FMath::Square(RadiusSum);
}

bool UIntersectionUtility::RaySphereIntersection(const FVector& Origin, const FVector& RayDirection,
	const float RayLength, const FVector& SphereCenter, const float SphereRadius)
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

	return true;
}

bool UIntersectionUtility::RayPlaneIntersection(const FVector& RayOrigin, const FVector& RayDirection, const float RayLength,
	const FVector& PlaneCenter, const FVector& PlaneNormal)
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
	
	return true;
}

bool UIntersectionUtility::RayAABBIntersection(const FVector& RayOrigin, const FVector& RayDirection,
	const float RayLength, const FVector& Min, const FVector& Max)
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
	
	return TMaxFinal >= 0 && TMinFinal <= RayLength && TMinFinal <= TMaxFinal;

}

bool UIntersectionUtility::AABBAABBIntersection(const FVector& MinA, const FVector& MaxA, const FVector& MinB,
	const FVector& MaxB)
{
	if(MinA.X > MaxB.X || MinB.X > MaxA.X)
		return false;
	if(MinA.Y > MaxB.Y || MinB.Y > MaxA.Y)
		return false;
	if(MinA.Z > MaxB.Z || MinB.Z > MaxA.Z)
		return false;

	return true;
}

bool UIntersectionUtility::PointAABBIntersection(const FVector& Point, const FVector& Min, const FVector& Max)
{
	if(Point.X > Max.X || Min.X > Point.X)
		return false;
	if(Point.Y > Max.Y || Min.Y > Point.Y)
		return false;
	if(Point.Z > Max.Z || Min.Z > Point.Z)
		return false;

	return true;
}

bool UIntersectionUtility::SphereAABBIntersection(const FVector& SphereCenter, const float SphereRadius, const FVector& AABBCenter, const FVector& AABBExtents, FVector& OutClosestPoint)
{
	const FVector LocalSphereCenter = SphereCenter - AABBCenter;

	for (int32 i = 0; i < 3; ++i)
	{
		const float component = LocalSphereCenter[i];

		if (component < -AABBExtents[i])
		{
			OutClosestPoint[i] = -AABBExtents[i];
		}
		else if (component > AABBExtents[i])
		{
			OutClosestPoint[i] = AABBExtents[i];
		}
		else
		{
			OutClosestPoint[i] = component;
		}
	}

	const float SquaredDistToClosest = (OutClosestPoint - LocalSphereCenter).SizeSquared();

	return SquaredDistToClosest <= (SphereRadius * SphereRadius);	
}

bool UIntersectionUtility::AABBPlaneIntersection(const FVector& AABBLocation, const FVector& AABBExtent,
	const FTransform& PlaneTransform, const FVector& PlaneNormal, const FVector2D PlaneExtent)
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

bool UIntersectionUtility::IsOnScreen(const APlayerController* PlayerController, const UCollisionComponent* Component,
	const float ScreenSpaceCompare)
{
	const FCollider Collider = Component->GetCollider();
	
	// If Sphere
	if(Collider.GetType() == EColliderType::Sphere)
	{
		const FSphereCollider SphereCollider = static_cast<const FSphereCollider&>(Collider);
		return IsSphereOnScreen(Component->GetOwner()->GetActorLocation(), SphereCollider.Radius, PlayerController);
	}
	// TODO If Plane
	// if(Collider.GetType() == EColliderType::Plane)
	// {
	// 	const FPlaneCollider PlaneCollider = static_cast<const FPlaneCollider&>(Collider);
	// 	return IsPlaneOnScreen(Component->GetOwner()->GetActorLocation(), PlaneCollider.GetNormal(), PlaneCollider.Extent, PlayerController);
	// }
	return false;
}

bool UIntersectionUtility::IsPointOnScreen(const APlayerController* PlayerController, const FVector& WorldPosition,
										   const float ScreenSpaceCompare)
{
	FVector2d ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(PlayerController, WorldPosition, ScreenPosition);

	int32 ViewportSizeX, ViewportSizeY;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	return (ScreenPosition.X > ScreenSpaceCompare && ScreenPosition.X < ViewportSizeX - ScreenSpaceCompare) &&
		(ScreenPosition.Y > ScreenSpaceCompare && ScreenPosition.Y < ViewportSizeY - ScreenSpaceCompare);
}

bool UIntersectionUtility::IsAABBOnScreen(const FVector& Min, const FVector& Max, const APlayerController* PlayerController,
	const float ScreenCompare)
{
	return IsPointOnScreen(PlayerController, Min, ScreenCompare)
		&& IsPointOnScreen(PlayerController, Max, ScreenCompare);
}


bool UIntersectionUtility::IsSphereOnScreen(const FVector& Center, const float Radius,
                                            const APlayerController* PlayerController, const float ScreenCompare)
{
	// TODO add radius to the check
	return IsPointOnScreen(PlayerController, Center, ScreenCompare );	
}

bool UIntersectionUtility::IsPlaneOnScreen(const FVector& Center, const FVector& Normal, const FVector2d Bounds,
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

bool UIntersectionUtility::RayTriangleIntersection(const FVector& Origin, const FVector& Direction, const FVector& V0,
	const FVector& V1, const FVector& V2)
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
	
	return T >= 0.f;
}

bool UIntersectionUtility::SphereTriangleIntersection(const FVector& SphereCenter, float SphereRadius,
	const FVector& V0, const FVector& V1, const FVector& V2)
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
	if (!PointTriangleIntersection(ProjectedCenter, V0, V1, V2)) {
		const FVector ProjectedCenterToV0 = V0 - ProjectedCenter;
		if(PointTriangleIntersection(ProjectedCenterToV0, V0, V1, V2))
		{
			const FVector ProjectedCenterToV1 = V1 - ProjectedCenter;
			if(PointTriangleIntersection(ProjectedCenterToV1, V0, V1, V2))
			{
				const FVector ProjectedCenterToV2 = V2 - ProjectedCenter;
				if(PointTriangleIntersection(ProjectedCenterToV2, V0, V1, V2))
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool UIntersectionUtility::PointTriangleIntersection(const FVector& Point, const FVector& V0, const FVector& V1,
	const FVector& V2)
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
