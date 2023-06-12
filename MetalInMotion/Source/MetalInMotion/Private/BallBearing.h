// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "BallBearing.generated.h"

UCLASS()
class ABallBearing : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallBearing();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BallBearing)
		UStaticMeshComponent* BallMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BallBearing)
		bool Magnetized = true;

	friend class ABallBearingHUD;

};
