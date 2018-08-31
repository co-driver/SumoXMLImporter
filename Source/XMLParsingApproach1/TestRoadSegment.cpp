
#include "TestRoadSegment.h"
#include <map>
#include <stack>
#include <iostream>
#include <deque>
#include <algorithm>
#include <iterator>
#include <sstream>

const float pi = std::acos(-1);

// Sets default values
ATestRoadSegment::ATestRoadSegment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
	mesh->bUseAsyncCooking = true;
	

}

//ATestRoadSegment::ATestRoadSegment(FVector _vertex1, FVector _vertex2, FVector _vertex3, FVector _vertex4)
//{
//	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
//	RootComponent = mesh;
//	mesh->bUseAsyncCooking = true;
//
//	vertex1 = _vertex1;
//	vertex2 = _vertex2;
//	vertex3 = _vertex3;
//	vertex4 = _vertex4;
//
//
//}


void ATestRoadSegment::CreateFace(FVector vertex1b, FVector vertex2b, FVector vertex3b, FVector vertex4b, int32 indexb)
{
	UE_LOG(LogEngine, Warning, TEXT("The first vertex x coordinate is (inside class) %f"), vertex1.X);
	/*vertices.Add(vertex1);
	vertices.Add(vertex2);
	vertices.Add(vertex3);
	vertices.Add(vertex4);	*/		//TArray of vertices

	


	//TArray of triangles contains which corresponding index in the vertices array to use to form a triangle (sequence of 3 vertices at a time)
	
	//int secTriangle;
	
	//Triangles are formed taking vertex coordinates in an anticlockwise manner
	//
	if (1)
	{
	/*	Triangles.Add(0);
		Triangles.Add(1);
		Triangles.Add(2);

		Triangles.Add(0);
		Triangles.Add(2);
		Triangles.Add(3);
	*/

/*		Triangles.Add(2);
		Triangles.Add(0);
		Triangles.Add(1);

		Triangles.Add(3);
		Triangles.Add(0);
		Triangles.Add(2);
	*/
		Triangles.Add(0);
		Triangles.Add(3);
		Triangles.Add(1);

		Triangles.Add(1);
		Triangles.Add(3);
		Triangles.Add(2);



	}
	else
	{
		//UE_LOG(LogEngine, Warning, TEXT("This is a returning edge"));
		Triangles.Add(0);
		Triangles.Add(2);
		Triangles.Add(1);

		Triangles.Add(2);
		Triangles.Add(3);
		Triangles.Add(1);
		
	}


	normals.Add(FVector(0, 0, 1));
	normals.Add(FVector(0, 0, 1));
	normals.Add(FVector(0, 0, 1));
	normals.Add(FVector(0, 0, 1));



	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(1, 0));
	UV0.Add(FVector2D(0, 1));




	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));
	tangents.Add(FProcMeshTangent(0, 1, 0));



	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

}

void ATestRoadSegment::CreateSection(FVector vertex1a, FVector vertex2a, FVector vertex3a, FVector vertex4a, int32 indexa)
{
	CreateFace(vertex1a, vertex2a, vertex3a, vertex4a, indexa);

	mesh->CreateMeshSection_LinearColor(indexa, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

	//CreateTriangle(FVector(0, 0, 0), FVector(0, 0, 100), FVector(100, 0, 0), FVector(100, 0, 100), FVector(100, 0, 0), FVector(0, 0, 100));

	//mesh->CreateMeshSection_LinearColor(index, vertices, Triangles, normals, UV0, vertexColors, tangents, true);


	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);

	UE_LOG(LogEngine, Warning, TEXT("In CreateSection"));
}


void ATestRoadSegment::PostActorCreated()
{
	Super::PostActorCreated();

	//Reset All Tarrays
	vertices.Reset(0);
	Triangles.Reset(0);
	normals.Reset(0);
	UV0.Reset(0);
	tangents.Reset(0);
	vertexColors.Reset(0);

	////CreateSection(FVector(0, 0, 0), FVector(10, 0, 0), FVector(10, 10, 0), FVector(0, 10, 0), 0); //Bottom Face
	//CreateSection(FVector(0, 0, 0), FVector(0, 0, 10), FVector(10, 0, 10), FVector(10, 0, 0), 1); //Back
	//CreateSection(FVector(0, 10, 0), FVector(10, 10, 0), FVector(10, 10, 10), FVector(0, 10, 10), 2); //Front
	//CreateSection(FVector(0, 10, 10), FVector(10, 10, 10), FVector(10, 0, 10), FVector(0, 0, 10), 3); //Top
	//CreateSection(FVector(0, 10, 10), FVector(0, 0, 10), FVector(0, 0, 0), FVector(0, 10, 0), 4); //Left
	//CreateSection(FVector(10, 0, 10), FVector(10, 10, 10), FVector(10, 10, 0), FVector(10, 0, 0), 5);
	//UE_LOG(LogEngine, Warning, TEXT("the vertex1 x coordinate is %f"), ((this->vertex1).X));
	//CreateSection(vertex1, vertex2, vertex3, vertex4, 0); //4 vertices for forming the lane
}

void ATestRoadSegment::PostLoad()
{
	Super::PostLoad();

	////CreateSection(FVector(0, 0, 0), FVector(10, 0, 0), FVector(10, 10, 0), FVector(0, 10, 0), 0); //Bottom Face
	//CreateSection(FVector(0, 0, 0), FVector(0, 0, 10), FVector(10, 0, 10), FVector(10, 0, 0), 1); //Back
	//CreateSection(FVector(0, 10, 0), FVector(10, 10, 0), FVector(10, 10, 10), FVector(0, 10, 10), 2); //Front
	//CreateSection(FVector(0, 10, 10), FVector(10, 10, 10), FVector(10, 0, 10), FVector(0, 0, 10), 3); //Top
	//CreateSection(FVector(0, 10, 10), FVector(0, 0, 10), FVector(0, 0, 0), FVector(0, 10, 0), 4); //Left
	//CreateSection(FVector(10, 0, 10), FVector(10, 10, 10), FVector(10, 10, 0), FVector(10, 0, 0), 5); //Right

	//CreateSection(vertex1, vertex2, vertex3, vertex4, 0);
	

}
// Called when the game starts or when spawned
void ATestRoadSegment::BeginPlay()
{
	CreateSection(vertex1, vertex2, vertex3, vertex4, 0);
	Super::BeginPlay();

}

// Called every frame
void ATestRoadSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}





