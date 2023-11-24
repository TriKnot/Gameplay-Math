#include "Actor/NoiseFloor.h"

#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"


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
	
	// GetWorld()->GetSubsystem<UIntersectionSubsystem>()->RegisterNoiseTerrain(this);
}

void ANoiseFloor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
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
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("NoiseFloor: %s"), *Offset.ToString()));
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
