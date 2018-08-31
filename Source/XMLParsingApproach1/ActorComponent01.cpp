// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "ActorComponent01.h"
#include "Runtime/XmlParser/Public/FastXml.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "TestRoadSegment.h"
#include "Engine.h"
#include <math.h>
#include <cmath>
#include <map>
#include <stack>
#include <iostream>
#include <deque>
#include <algorithm>
#include <iterator>
#include <sstream>

//#include "strtk.hpp"






// Sets default values for this component's properties
UActorComponent01::UActorComponent01()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorComponent01::BeginPlay()
{
	Super::BeginPlay();
	loadxml();
	// ...
	
}


// Called every frame
void UActorComponent01::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
FText outError;
int32 outErrorNum;

bool UActorComponent01::loadxml()
{
	UE_LOG(LogEngine, Warning, TEXT("Lets Goooo"));
	FString XML = "C:/Users/iparanja/net.net.xml";
	bool success = FFastXml::ParseXmlFile((IFastXmlCallback*)this, XML.GetCharArray().GetData(), TEXT(""), nullptr, false, false, outError, outErrorNum);
	return success;
}

bool UActorComponent01::ProcessXmlDeclaration(const TCHAR* ElementData, int32 XmlFileLineNumber)
{
	UE_LOG(LogEngine, Warning, TEXT("ProcessXmlDeclaration ElementData: %s, XmlFileLineNumber: %f"), ElementData, XmlFileLineNumber);
	return true;
}

std::stack<const TCHAR*> junctionidStack;
std::stack<const TCHAR*> ElementNameStack;
std::stack<const TCHAR*> EdgeElementStack;

bool UActorComponent01::ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber)
{	
	UE_LOG(LogEngine, Warning, TEXT("ProcessElement ElementName: %s, ElementValue: %s"), ElementName, ElementData);
	
	ElementNameStack.push(ElementName);
	
	return true;
}
const TCHAR* ElementNameMap;
FVector LocationNode;
FVector sourceNode;
FVector targetNode;
FVector centralNode;
FRotator RotationNode(0.0f, 0.0f, 0.0f);
FVector pointA;		//starting point of the road
FVector pointB;		//ending point of the road

FVector LocationEdge;
FQuat RotationEdge01(0.0f, 0.0f, 0.0f, 0.0f);
FVector RotationEdge(0.0f, 0.0f, 0.0f);
const float pi = std::acos(-1);

//float roadLength;


const TCHAR* LatestJunctionid;


std::map <const TCHAR*, std::map <const TCHAR*, const TCHAR*>> MapofElementMaps; // Element-AttributeMap Map

std::map <const TCHAR*, std::map <const TCHAR*, const TCHAR*>> EdgeAttributeMap; //to store the edges
const TCHAR* edgeElement;

int32 flagNode = 0;
int32 flagEdge = 0;
int32 notPriorityFlag = 0;
int32 isInternalFlag = 0;
int32 yIsZeroFlag = 0;
int32 returnFlag;
FVector origin(0.0f, 0.0f, 0.0f);
FVector midpoint;
double midpointAngle;

TArray <float> Cuboid;
float roadLength;
void extractIntegers(std::string str)
{
	Cuboid.Empty();

	for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
		if (*it == ',') {
			*it = ' ';
		}
		else continue;//convert all commas in string stream into space
	}
	std::stringstream ss;
	ss << str;
	

	/* Running loop till the end of the stream */
	//std::string temp;
	float found;
	
	while (!ss.eof()) {

		/* extracting word by word from stream */
		//ss >> temp;

		/* Checking the given word is integer or not */
		if (ss >> found)
		{
			//UE_LOG(LogEngine, Warning, TEXT("shape value %f"), found);
			Cuboid.Add(found);
		}
			

		/* To save from space at the end of string */
		//temp = "";
	}
}

UWorld* world;

bool UActorComponent01::ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue)
{
	
	

	std::map <const TCHAR*, const TCHAR*> AttributeMap; //Attribute value map
	//UE_LOG(LogEngine, Warning, TEXT("ProcessAttribute AttributeName: %s, AttributeValue: %s"), AttributeName, AttributeValue);
	if (!ElementNameStack.empty())
	{
		ElementNameMap = ElementNameStack.top();
		ElementNameStack.pop();
		//UE_LOG(LogEngine, Warning, TEXT("ElementNameMap is %s"), ElementNameMap);
	}
	//MapofElementMaps.insert(std::pair <const TCHAR*, std::map <const TCHAR*, const TCHAR*>>(ElementNameMap, AttributeMap));
	

	FString NewAttributeName = (FString)AttributeName;
	FString NewElementName = (FString)ElementNameMap;

	
	std::string ss;
	//int32 w = 10;

	if (NewElementName.Equals(TEXT("junction")))
	{

		if (NewAttributeName.Equals(TEXT("id")))
		{
			junctionidStack.push(AttributeValue);

			

		}

		if (((FString)AttributeName).Equals(TEXT("type")))
		{
			if (((FString)AttributeValue).Equals(TEXT("priority")) || ((FString)AttributeValue).Equals(TEXT("unregulated")))
			{
				notPriorityFlag = 0;
			}

			else
			{
				notPriorityFlag = 1;
				while (!junctionidStack.empty())
					junctionidStack.pop();
			}
			
			
		}

		

		if ((NewAttributeName.Equals(TEXT("y")) || NewAttributeName.Equals(TEXT("x"))) && (notPriorityFlag == 0))
		{
			if (NewAttributeName.Equals(TEXT("x")))
			{
				
				FString newAttributeValue = (FString)AttributeValue;
				float newX = FCString::Atof(*newAttributeValue);
				LocationNode.X = newX;
				
			}

			else if (NewAttributeName.Equals(TEXT("y")))
			{
							
				FString newAttributeValue = (FString)AttributeValue;
				float newY = FCString::Atof(*newAttributeValue);
				LocationNode.Y = newY;
				
			}

			

			if (!junctionidStack.empty())
			{
				LatestJunctionid = junctionidStack.top();
				junctionidStack.pop();  //pop the latest junction id

				UE_LOG(LogEngine, Warning, TEXT("Junction id is %s"), LatestJunctionid);
			}

			MapofElementMaps[LatestJunctionid].insert(std::pair <const TCHAR*, const TCHAR*>(AttributeName, AttributeValue));


			const FActorSpawnParameters SpawnInfo;
			if (NewAttributeName.Equals(TEXT("y")))  //since y comes after x, spawn after y is set
			{
				UE_LOG(LogEngine, Warning, TEXT("Location node is %f %f"), LocationNode.X, LocationNode.Y);

				GetWorld()->SpawnActor<ATestRoadSegment>(LocationNode, RotationNode, SpawnInfo);
			}

			
			
			//UE_LOG(LogEngine, Warning, TEXT("Junction Actor Spawned!"));
			//UE_LOG(LogEngine, Warning, TEXT("Junction Actor X coordinate at %f"), LocationNode.X);
			//UE_LOG(LogEngine, Warning, TEXT("Junction Actor Y Coordinate at %f"), LocationNode.Y);
			
			flagNode = 1;
		}
	}
			// Printing Attribute Map
			//std::map <const TCHAR*, const TCHAR*> ::iterator itr;
			//UE_LOG(LogEngine, Warning, TEXT("\nThe map is : \n"));

			//for (itr = AttributeMap.begin(); itr != AttributeMap.end(); ++itr)
			//{
			//	UE_LOG(LogEngine, Warning, TEXT("Attribute Name is %s"), itr->first);
			//	UE_LOG(LogEngine, Warning, TEXT("Attribute Value is %s"), itr->second);
			//}}
		

	else if (NewElementName.Equals(TEXT("edge"))) //parse edges, store them in map first (since edges may be parsed before junctions)
	{
		
		if (NewAttributeName.Equals(TEXT("id")))
		{
			UE_LOG(LogEngine, Warning, TEXT("Edge id spotted"));
			EdgeElementStack.push(AttributeValue);

		}

		if (NewAttributeName.Equals(TEXT("function")))
		{
			if (((FString)AttributeValue).Equals(TEXT("internal")))
			{
				isInternalFlag = 1;
				while (!EdgeElementStack.empty())
					EdgeElementStack.pop();
			}
		}
		else
		{
			isInternalFlag = 0;
		}

		if ((NewAttributeName.Equals(TEXT("from")) || NewAttributeName.Equals(TEXT("to"))) || NewAttributeName.Equals(TEXT("shape")) && (isInternalFlag == 0))
		{
			
			//UE_LOG(LogEngine, Warning, TEXT("Edge from spotted"));
			if (!EdgeElementStack.empty()) // store edge id only if that id has a 'from' and 'to' attribute
			{
				edgeElement = EdgeElementStack.top();
				EdgeElementStack.pop();		//pop latest edge element and store in map

				EdgeAttributeMap.insert(std::pair <const TCHAR*, std::map<const TCHAR*, const TCHAR*>>(edgeElement, AttributeMap));
			}

			std::map <const TCHAR*, std::map <const TCHAR*, const TCHAR*>> ::iterator itr;

			/*if (NewAttributeName.Equals(TEXT("shape")))
			{
				FString shapeString = (FString)AttributeValue;

				std::deque<int> coordinate_list;

				strtk::parse(shapeString, " |,", coordinate_list);

				for (auto coordinate : coordinate_list)
				{

					UE_LOG(LogEngine, Warning, TEXT("shape coordinate %d"), coordinate);

				}

			}*/

			for (itr = EdgeAttributeMap.begin(); itr != EdgeAttributeMap.end(); itr++)
			{
				if (((FString)(itr->first)).Equals((FString)edgeElement)) //find that most recently added id
				{
					(itr->second).insert(std::pair <const TCHAR*, const TCHAR*>(AttributeName, AttributeValue));
					if (((FString)AttributeName).Equals(TEXT("to")))
					{
						flagEdge = 1; //once a 'from' and 'to' make flagedge = 1 to show that 
						UE_LOG(LogEngine, Warning, TEXT("New Edge Stored"));
					}
					
				}
			}

			std::map <const TCHAR*, std::map <const TCHAR*, const TCHAR*>> ::iterator itr02;
			std::map <const TCHAR*, const TCHAR*> ::iterator itr02_inner;

			for (itr02 = EdgeAttributeMap.begin(); itr02 != EdgeAttributeMap.end(); itr02++)
			{
				if (((FString)AttributeName).Equals(TEXT("to")))
				{
					for (itr02_inner = (itr02->second).begin(); itr02_inner != (itr02->second).end(); itr02_inner++)
					{
						if (((FString)(itr02_inner->first)).Equals(TEXT("from")))
						{
							if (((FString)(itr02_inner->second)).Equals((FString)AttributeValue))
							{
								//returnFlag += 1;
								
							}
						}
					}
				}
			}

		}

	}

	
	else if (NewElementName.Equals(TEXT("lane")) && (isInternalFlag == 0)) //lane is checked after a 'non-internal' edge is parsed
	{
		int32 w = 2;
		FVector Ax0;
		FVector Ax1;
	
		FVector Bx0;
		FVector Bx1;

		

		


		float xOffset;
		float yOffset;

		float theta;
		float intermediateTheta;
	


		if (NewAttributeName.Equals(TEXT("length")))
		{
			roadLength = FCString::Atof(AttributeValue); // from x coordinate
			UE_LOG(LogEngine, Warning, TEXT("The road length is %f"), roadLength);
		}
		if (NewAttributeName.Equals(TEXT("shape")))
		{
			TArray<FVector> vertexArray; //Array to store vertices


			ss = std::string(TCHAR_TO_UTF8(AttributeValue));
			extractIntegers(ss); //parse and print the numbers in the shape string
			for (int i=0; i<Cuboid.Num(); i++)
			{
				UE_LOG(LogEngine, Warning, TEXT("Cuboid[%d] = %f"), i, Cuboid[i]);
			}
			intermediateTheta = ((Cuboid[3] - Cuboid[1]) / roadLength);
			if (intermediateTheta > 1.0) intermediateTheta = 1.0; // clamp domain to -1 .. 1
			if (intermediateTheta < -1.0) intermediateTheta = -1.0;

			theta = std::asin(intermediateTheta);
			


			//UE_LOG(LogEngine, Warning, TEXT("The value of Cuboid is %f, %f, %f, %f"), Cuboid[0], Cuboid[1], Cuboid[2], Cuboid[3]);
			UE_LOG(LogEngine, Warning, TEXT("theta is %f"), theta);
			xOffset = ((w / 2)*(std::cos((pi/2) - theta)));
			UE_LOG(LogEngine, Warning, TEXT("The value of xOffset is %f"), xOffset);

			yOffset = ((w / 2)*(std::sin((pi/2) - theta)));
			UE_LOG(LogEngine, Warning, TEXT("The value of yOffset is %f"), yOffset);

			Ax0.X = Cuboid[0] - xOffset;
			//UE_LOG(LogEngine, Warning, TEXT("The value of vertex1 x is %f"), Ax0.X);
			Ax0.Y = Cuboid[1] + yOffset;
			//UE_LOG(LogEngine, Warning, TEXT("The value of the vertex1 y is %f"), Ax0.Y);
			Ax0.Z = 0.0f;
			UE_LOG(LogEngine, Warning, TEXT("Vertex 1: (%f,%f)"), Ax0.X, Ax0.Y);
			vertexArray.Add(Ax0);

			Ax1.X = Cuboid[0] + xOffset;
			//UE_LOG(LogEngine, Warning, TEXT("The value of the vertex2 x is %f"), Ax1.X);
			Ax1.Y = Cuboid[1] - yOffset;
			//UE_LOG(LogEngine, Warning, TEXT("The value of the vertex2 y is %f"), Ax1.Y)
			Ax1.Z = 0.0f;
			UE_LOG(LogEngine, Warning, TEXT("Vertex 2: (%f,%f)"), Ax1.X, Ax1.Y);
			vertexArray.Add(Ax1);


			Bx0.X = Cuboid[2] - xOffset;
			//UE_LOG(LogEngine, Warning, TEXT("The value of the vertex3 x is %f"), Bx0.X);
			Bx0.Y = Cuboid[3] + yOffset;
			//UE_LOG(LogEngine, Warning, TEXT("The value of the vertex3 y is %f"), Bx0.Y);

			Bx0.Z = 0.0f;
			UE_LOG(LogEngine, Warning, TEXT("Vertex 3: (%f,%f)"), Bx0.X, Bx0.Y);
			vertexArray.Add(Bx0);



			Bx1.X = Cuboid[2] + xOffset;
			Bx1.Y = Cuboid[3] - yOffset;
			Bx1.Z = 0.0f;

			UE_LOG(LogEngine, Warning, TEXT("Vertex 4: (%f,%f)"), Bx1.X, Bx1.Y);
			vertexArray.Add(Bx1);


			
			FVector* centroid = new FVector();

			centroid->X = 0.0f;
			centroid->Y = 0.0f;
			centroid->Z = 0.0f;

			for (int i = 0; i < vertexArray.Num(); i++)
			{
				*centroid += vertexArray[i];
			}

			centroid->X /= vertexArray.Num();	//Get the centroid vector (average of all 4 vertices)

			centroid->Y /= vertexArray.Num();

			
			UE_LOG(LogEngine, Warning, TEXT("The centroid vector is (%f, %f, %f)"), centroid->X, centroid->Y, centroid->Z);

			TArray<float> vertexAnglesUnSorted;
			TArray<float> vertexAnglesSorted;

			for (int i = 0; i < vertexArray.Num(); i++)
			{
				FVector dir = vertexArray[i] - *centroid;		//Direction vector A to B. Calculated using (FVector B - FVector A)

				//UE_LOG(LogEngine, Warning, TEXT("The direction vector is (%f, %f, %f)"), dir.X, dir.Y, dir.Z);

				//float angle2 = dir.HeadingAngle() + pi;
				float angle = std::atan2((vertexArray[i].Y - centroid->Y), (vertexArray[i].X - centroid->X))+pi;		//convert direction vector to heading angle to get it in 0-2pi range
				
				if (i == 3) angle -= 0.00001;  // hack to handle case where final two angles are identical, force ordering to 3 then 2
				
				float actualVertexIndex = (float)(i + 1); //just for logging purposes

				//UE_LOG(LogEngine, Warning, TEXT("The heading angle for vertex %f is %f"), actualVertexIndex, angle2);
				UE_LOG(LogEngine, Warning, TEXT("The atan2 angle for vertex %f is %f"), actualVertexIndex, angle);
				vertexAnglesUnSorted.Add(angle);		//Add vertex angles to a Tarray
			}

			vertexAnglesSorted = vertexAnglesUnSorted; //copy to another array

			vertexAnglesSorted.Sort(); //new array of sorted angles


			


			
			
			FTransform SpawnTransform(RotationEdge01, origin);
			ATestRoadSegment* MyDeferredActor = Cast<ATestRoadSegment>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ATestRoadSegment::StaticClass(), SpawnTransform)); //Downcasting

			if (MyDeferredActor)

			{
				for (int i = 0; i < vertexAnglesSorted.Num(); i++) //find the corresponding index of the sorted angle to the unsorted angle vertex
				{
					UE_LOG(LogEngine, Warning, TEXT("The sorted angle is %f"), vertexAnglesSorted[i]);
					for (int j = 0; j < vertexAnglesUnSorted.Num(); j++)
					{
						if (vertexAnglesUnSorted[j] == vertexAnglesSorted[i])
						{
							UE_LOG(LogEngine, Warning, TEXT("This corresponds to vertex %d"), (j + 1));
							(MyDeferredActor->vertices).Add(vertexArray[j]);
						}

					}
				}
				/*MyDeferredActor->vertex1 = Ax0;
				MyDeferredActor->vertex2 = Ax1;
				MyDeferredActor->vertex3 = Bx0;
				MyDeferredActor->vertex4 = Bx1;  *///Change the mesh actor's 4 vertex coordinates
				MyDeferredActor->returnEdge = returnFlag;
				UE_LOG(LogEngine, Warning, TEXT("The x coordinate of the first vertex is %f"), (MyDeferredActor->vertex1.X));
				UGameplayStatics::FinishSpawningActor(MyDeferredActor, SpawnTransform);
				//UE_LOG(LogEngine, Warning, TEXT("The x coordinate of the first vertex is %f"), (MyDeferredActor->vertex1.X));
				UE_LOG(LogEngine, Warning, TEXT("the edge actor is spawned"));
			}

			

			//Cuboid.Empty();  //clear the intial coordinate array
			returnFlag = 0;

				
					


			









		}

		

		//
		//
		//Cuboid[0] = pointA.X;
		//Cuboid[1] = pointA.Y;    //SpawnLocation is point A (first co-ordinate pair parsed) (check diagram)

		//Cuboid[2] = pointB.X;
		//Cuboid[3] = pointB.Y;	//Co-ordinates of point B


		

		//FTransform SpawnTransform(RotationEdge01, pointA);
		//ATestRoadSegment* MyDeferredActor = Cast<ATestRoadSegment>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ATestRoadSegment::StaticClass(), SpawnTransform)); //Downcasting

		//if (MyDeferredActor)
		//{
		//	MyDeferredActor->vertex1.X = Cuboid[0];
		//	MyDeferredActor->vertex1.Y = Cuboid[1];
		//	MyDeferredActor->vertex2.X = Cuboid[2];
		//	MyDeferredActor->vertex2.Y = Cuboid[3];  //Change the mesh actor's 4 vertex coordinates
		//	//MyDeferredActor->length = 250;
		//		//(FMath::Sqrt((FMath::Square(newX2 - newX1)) - (FMath::Square(newY2 - newY1))));;
		//	//UE_LOG(LogEngine, Warning, TEXT("The length of the edge is %d"), (MyDeferredActor->length));
		//	
		//}
				

    }

	
	//UE_LOG(LogEngine, Warning, TEXT("FlagEdge Value %d"), flagEdge);
	//UE_LOG(LogEngine, Warning, TEXT("FlagNode Value %d"), flagNode);

	



		
	return true;
}



bool UActorComponent01::ProcessClose(const TCHAR* Element)
{
	//test printing out values of junctions
	if (((FString)Element).Equals(TEXT("net")))
	{
		std::map <const TCHAR*, std::map <const TCHAR*, const TCHAR*>> ::iterator itr;
		for (itr = MapofElementMaps.begin(); itr != MapofElementMaps.end(); itr++)
		{
			UE_LOG(LogEngine, Warning, TEXT("New Junction"));
			UE_LOG(LogEngine, Warning, TEXT("id name is %s"), (itr->first));

			if (((FString)(itr->first)).Equals(TEXT("A1")))
			{

				std::map <const TCHAR*, const TCHAR*> ::iterator itr01;
				for (itr01 = (itr->second).begin(); itr01 != (itr->second).end(); itr01++)
				{
					UE_LOG(LogEngine, Warning, TEXT("The key is %s"), (itr01->first));
					if (((FString)(itr01->first)).Equals(TEXT("x")))
					{

						float targetX = FCString::Atof(itr01->second);
						centralNode.X = targetX;
					}

					else if (((FString)(itr01->first)).Equals(TEXT("y")))
					{
						float targetY = FCString::Atof(itr01->second);
						centralNode.Y = targetY;
					}
					//UE_LOG(LogEngine, Warning, TEXT("The corresponding value is %s"), (itr01->second));
				}

			}

		}
		if (((FString)Element).Equals(TEXT("net")))
		{
			UE_LOG(LogEngine, Warning, TEXT("ProcessClose Element: %s"), Element);
			if ((flagNode > 0) && (flagEdge > 0))
			{
				UE_LOG(LogEngine, Warning, TEXT("Spawning begins"));
				std::map <const TCHAR*, std::map <const TCHAR*, const TCHAR*>> ::iterator itrEdge;


				FString fromNodeid;
				FString toNodeid;

				FString x1;		//x coordinate of from node
				FString x2;		//x coordinate of to node

				FString y1;		//y coordinate of from node
				FString y2;		//y coordinate of to node


				for (itrEdge = EdgeAttributeMap.begin(); itrEdge != EdgeAttributeMap.end(); itrEdge++)//outer map for edges
				{
					//outer map for nodes
					std::map <const TCHAR*, const TCHAR*> ::iterator itrNodeAttribute;	//outer map for nodes


					std::map <const TCHAR*, const TCHAR*> ::iterator itrEdgeAttribute;

					for (itrEdgeAttribute = (itrEdge->second).begin(); itrEdgeAttribute != (itrEdge->second).end(); itrEdgeAttribute++) //inner map for edges
					{

						if (((FString)(itrEdgeAttribute->first)).Equals(TEXT("from")))
						{
							fromNodeid = (FString)(itrEdgeAttribute->second);
							UE_LOG(LogEngine, Warning, TEXT(" if1 retrieved"));

							std::map <const TCHAR*, std::map <const TCHAR*, const TCHAR*>> ::iterator itrNode;
							for (itrNode = MapofElementMaps.begin(); itrNode != MapofElementMaps.end(); itrNode++)//outer map for nodes
							{
								UE_LOG(LogEngine, Warning, TEXT("second for loop"));
								if (((FString)(itrNode->first)).Equals(fromNodeid)) //check if the 'from' node id matches the current node id
								{
									UE_LOG(LogEngine, Warning, TEXT("if2"));

									for (itrNodeAttribute = (itrNode->second).begin(); itrNodeAttribute != (itrNode->second).end(); itrNodeAttribute++) //traverse the attributes for that particular node
									{

										if (((FString)(itrNodeAttribute->first)).Equals(TEXT("x")))
										{
											UE_LOG(LogEngine, Warning, TEXT("Running x"));
											x1 = (FString)(itrNodeAttribute->second);
											UE_LOG(LogEngine, Warning, TEXT(" from node x coordinate retrieved is %s"), *x1);
										}
										else if (((FString)(itrNodeAttribute->first)).Equals(TEXT("y")))
										{
											UE_LOG(LogEngine, Warning, TEXT(""));
											y1 = (FString)(itrNodeAttribute->second);
											UE_LOG(LogEngine, Warning, TEXT(" from node y coordinate retrieved is %s"), *y1);

										}
									}
								}
							}
						}
						else if (((FString)(itrEdgeAttribute->first)).Equals(TEXT("to")))
						{
							toNodeid = (FString)(itrEdgeAttribute->second);
							std::map <const TCHAR*, std::map <const TCHAR*, const TCHAR*>> ::iterator itrNode;
							for (itrNode = MapofElementMaps.begin(); itrNode != MapofElementMaps.end(); itrNode++)//outer map for nodes
							{
								if (((FString)(itrNode->first)).Equals(toNodeid)) //check if the 'from' node id matches the current node id
								{
									for (itrNodeAttribute = (itrNode->second).begin(); itrNodeAttribute != (itrNode->second).end(); itrNodeAttribute++) //traverse the attributes for that particular node
									{
										if (((FString)(itrNodeAttribute->first)).Equals(TEXT("x")))
										{
											x2 = (FString)(itrNodeAttribute->second);
											UE_LOG(LogEngine, Warning, TEXT(" to node x coordinate retrieved at %s"), *x2);

										}
										else if ((((FString)(itrNodeAttribute->first)).Equals(TEXT("y"))))
										{
											y2 = (FString)(itrNodeAttribute->second);
											UE_LOG(LogEngine, Warning, TEXT(" to node y coordinate retrieved at %s"), *y2);

										}
									}
								}
							}
						}
					}

					float newX1 = FCString::Atof(*x1); // from x coordinate
					float newX2 = FCString::Atof(*x2); // to x coordinate

					float newY1 = FCString::Atof(*y1); // from y coordinate
					float newY2 = FCString::Atof(*y2); // to y coordinate

					LocationEdge.X = ((newX1 + newX2) / 2);
					LocationEdge.Y = ((newY1 + newY2) / 2); //Set the location vector for edge

					//UE_LOG(LogEngine, Warning, TEXT("Edge Actor Spawned at x coordinate %f"), LocationEdge.X);
					//UE_LOG(LogEngine, Warning, TEXT("Edge Actor Spawned at y coordinate %f"), LocationEdge.Y);

					//FTransform SpawnTransform(RotationEdge01, LocationEdge);
					//ATestRoadSegment* MyDeferredActor = Cast<ATestRoadSegment>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ATestRoadSegment::StaticClass(), SpawnTransform)); //Downcasting

					//if (MyDeferredActor)
					//{
					//	sourceNode = LocationEdge - centralNode;
					//	MyDeferredActor->SetActorRotation(sourceNode.Rotation());
					//	//MyDeferredActor->length = 250;
					//		//(FMath::Sqrt((FMath::Square(newX2 - newX1)) - (FMath::Square(newY2 - newY1))));;
					//	//UE_LOG(LogEngine, Warning, TEXT("The length of the edge is %d"), (MyDeferredActor->length));
					//	UGameplayStatics::FinishSpawningActor(MyDeferredActor, SpawnTransform);
					//}
				}
			}
		}
	}


	return true;
}

bool UActorComponent01::ProcessComment(const TCHAR* Comment)
{
	UE_LOG(LogEngine, Warning, TEXT("ProcessComment Comment: %s"), Comment);
	return true;
}





