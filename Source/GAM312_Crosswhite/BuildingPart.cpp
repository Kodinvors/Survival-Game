// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPart.h"

// Sets default values
ABuildingPart::ABuildingPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates the mesh for the building part
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// Sets the pivot point for the build part
	PivotArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Pivot Arrow"));

	// Assigns the PivotArrow to the RootComponent
	RootComponent = PivotArrow;
	// Attaches the mesh to the PivotArrow
	Mesh->SetupAttachment(PivotArrow);

}

// Called when the game starts or when spawned
void ABuildingPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

