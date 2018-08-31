// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "TestRoadSegment.generated.h"

UCLASS()
class XMLPARSINGAPPROACH1_API ATestRoadSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestRoadSegment();
	//ATestRoadSegment(FVector _vertex1, FVector _vertex2, FVector _vertex3, FVector _vertex4);
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent *mesh;
	TArray<FVector> vertices;
	TArray<int32> Triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	FVector vertex1;
	FVector vertex2;
	FVector vertex3;
	FVector vertex4;

	int32 returnEdge;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	//void CreateTriangle(FVector, FVector, FVector, FVector, FVector, FVector, FVector, FVector);
	void CreateFace(FVector, FVector, FVector, FVector, int32);
	//void CreateMesh(FVector, FVector, FVector, FVector, FVector, FVector, FVector, FVector, int32);
	void CreateSection(FVector, FVector, FVector, FVector, int32);
};
