// Fill out your copyright notice in the Description page of Project Settings.


#include "BallBearing.h"

// Sets default values
ABallBearing::ABallBearing()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetSimulatePhysics(true);
	SetRootComponent(BallMesh);
}

/**
Called when the game starts or when spawned.
*********************************************************************************/

void ABallBearing::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = BallMesh->GetComponentLocation();

	BallMesh->SetLinearDamping(0.5f);
	BallMesh->SetAngularDamping(0.5f);
}

/**
Control the movement of the ball bearing, called every frame.
*********************************************************************************/

void ABallBearing::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	InContact = false;
}
