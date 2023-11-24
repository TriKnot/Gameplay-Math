#include "Actor/NoiseFloor.h"

#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Subsystems/CollisionSubsystem.h"


ANoiseFloor::ANoiseFloor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh");
	ProceduralMeshComponent->SetupAttachment(GetRootComponent());
}

void ANoiseFloor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GenerateNoiseMap();
	GenerateMesh();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
		Vertices, Triangles, UV, Normals, Tangents);

	ProceduralMeshComponent->CreateMeshSection(
		0,
		Vertices,
		Triangles,
		Normals,
		UV,
		Colors,
		Tangents,
		false
		);

	ProceduralMeshComponent->SetMaterial(
		0,
		Material
		);
}


void ANoiseFloor::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateNoiseMap();
	GenerateMesh();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV, Normals, Tangents);

	ProceduralMeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UV, Colors, Tangents, true);
	ProceduralMeshComponent->SetMaterial(0, Material);
	
	GetWorld()->GetSubsystem<UCollisionSubsystem>()->RegisterNoiseFloor(this);
}

void ANoiseFloor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetSubsystem<UCollisionSubsystem>()->UnregisterNoiseFloor();
}

void ANoiseFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float x = FMath::Sin(GetWorld()->GetTimeSeconds());
	const float y = FMath::Cos(GetWorld()->GetTimeSeconds());
	Offset += FVector2D(x, y) * DeltaTime * WaveSpeed;

	GenerateNoiseMap();
	GenerateMesh();

	ProceduralMeshComponent->UpdateMeshSection(
		0,
		Vertices,
		Normals,
		UV,
		Colors,
		Tangents
		);
	
}

void ANoiseFloor::GenerateNoiseMap()
{
	NoiseValues.SetNum((Resolution + 1) * (Resolution + 1));

	for (auto Y = 0; Y <= Resolution; Y++)
	{
		for (auto X = 0; X <= Resolution; X++)
		{
			const float SampleX = (static_cast<float>(X) + Offset.X + 0.1f) * NoiseScale;
			const float SampleY = (static_cast<float>(Y) + Offset.Y + 0.1f) * NoiseScale;

			const FVector2D Position = FVector2D(SampleX, SampleY);
			
			const int32 Index = Y * Resolution + X;
			NoiseValues[Index] = FMath::PerlinNoise2D(Position) * Strength;
		}
	}
}

void ANoiseFloor::GenerateMesh()
{
	Vertices.SetNum((Resolution + 1) * (Resolution + 1));
	Normals.SetNum(Vertices.Num());
	UV.SetNum(Vertices.Num());
	Colors.SetNum(Vertices.Num());
	Triangles.SetNum(Resolution * Resolution * 6);

	const float StepSize = 1.f / Resolution;

	for(int v = 0, y = 0; y <= Resolution; y++)
	{
		for(int x = 0; x <= Resolution; x++, v++)
		{
			const float Height = NoiseValues[x + y * Resolution];
			Vertices[v] = FVector(x * StepSize - 0.5f, y * StepSize - 0.5f, Height) * FloorScale;
			UV[v] = FVector2D(x * StepSize, y * StepSize);
		}
	}

	for(int t = 0, v = 0, y = 0; y < Resolution; y++, v++)
	{
		for(int x = 0; x < Resolution; x++, v++, t += 6)
		{
			Triangles[t] = v;
			Triangles[t + 1] = v + Resolution + 1;
			Triangles[t + 2] = v + 1;
			Triangles[t + 3] = v + Resolution + 1;
			Triangles[t + 4] = v + Resolution + 2;
			Triangles[t + 5] = v + 1;
		}
	}

	CalculateNormals();
}

void ANoiseFloor::CalculateNormals()
{
	for (int v = 0, y = 0; y <= Resolution; y++)
	{
		for (int x = 0; x <= Resolution; x++, v++)
		{
			FVector Normal = FVector::UpVector;
			Normal.Z = FMath::Square(1.f - (Normal.X * Normal.X + Normal.Y * Normal.Y));
			Normals[v] = Normal;
		}
	}
}

void ANoiseFloor::FindClosestTriangle(const FVector& Point, TArray<FVector>& ReturnTriangle)
{
	// Calculate the closest triangle to the given point
	float ClosestDistanceSquared = MAX_FLT;
	int32 ClosestTriangleIndex = -1;

	// Iterate through triangles
	for (int32 TriangleIndex = 0; TriangleIndex < Triangles.Num(); TriangleIndex += 3)
	{
		// Get the vertices of the triangle
		const FVector& V0 = Vertices[Triangles[TriangleIndex]];
		const FVector& V1 = Vertices[Triangles[TriangleIndex + 1]];
		const FVector& V2 = Vertices[Triangles[TriangleIndex + 2]];

		// Calculate the distance between the point and the triangle
		FVector Middle = (V0 + V1 + V2) / 3.f;
		const float DistanceSquared = FVector::DistSquared(Point, Middle);

		// Check if this triangle is closer
		if (DistanceSquared < ClosestDistanceSquared)
		{
			ClosestDistanceSquared = DistanceSquared;
			ClosestTriangleIndex = TriangleIndex / 3; // Store the triangle index
		}
	}

	if (ClosestTriangleIndex != -1)
	{
		// Add the indices of the vertices of the closest triangle to the output array
		const FVector ActorLocation = GetActorLocation();
		ReturnTriangle.Add(Vertices[Triangles[ClosestTriangleIndex * 3]] + ActorLocation);
		ReturnTriangle.Add(Vertices[Triangles[ClosestTriangleIndex * 3 + 1]]+ ActorLocation);
		ReturnTriangle.Add(Vertices[Triangles[ClosestTriangleIndex * 3 + 2]]+ ActorLocation);
	}
}


