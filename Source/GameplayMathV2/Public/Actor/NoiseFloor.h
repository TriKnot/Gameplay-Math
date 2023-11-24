#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "NoiseFloor.generated.h"

UCLASS(hidecategories=(Input, Movement, Collision, Rendering, HLOD, WorldPartition, DataLayers, Replication, Physics, Networking, Actor, LevelInstance, Cooking))
class GAMEPLAYMATHV2_API ANoiseFloor : public AActor
{
	GENERATED_BODY()
	
public:
	ANoiseFloor();

	void GenerateNoiseMap();
	void CalculateNormals();
	void GenerateMesh();
	void FindClosestTriangle(const FVector& Point, TArray<FVector>& ReturnTriangle);
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise", meta=(AllowPrivateAccess="true"))
	FVector FloorScale = FVector(100.f, 100.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise", meta=(AllowPrivateAccess="true"))
	FVector2D Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise", meta=(AllowPrivateAccess="true"))
	int32 Resolution = 128;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise", meta=(AllowPrivateAccess="true"))
	float NoiseScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Noise", meta=(AllowPrivateAccess="true"))
	float Strength = 1.f;

	UPROPERTY(EditAnywhere	, BlueprintReadWrite, Category="Noise", meta=(AllowPrivateAccess="true"))
	float WaveSpeed = 0.1f;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	TArray<float> NoiseValues;

	UPROPERTY()
	UProceduralMeshComponent* ProceduralMeshComponent;

	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FVector2D> UV;
	TArray<FColor> Colors;
	TArray<int32> Triangles;
	TArray<FProcMeshTangent> Tangents;
	
};
