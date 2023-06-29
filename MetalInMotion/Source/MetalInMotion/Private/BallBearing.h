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


	UFUNCTION(BlueprintCallable, Category = "Ball Bearing")
		void ResetLocation() const {

		BallMesh->SetWorldLocation(InitialLocation + FVector(0.0f, 0.0f, 150.0f));
		BallMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		BallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	}

protected:

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Control the movement of the ball bearing, called every frame.
	virtual void Tick(float deltaSeconds) override;

	// Receive notification of a collision contact and record that we're in contact with something.
	virtual void NotifyHit(UPrimitiveComponent* myComponent, AActor* other, UPrimitiveComponent* otherComp, bool selfMoved, FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hitResult) override
	{
		Super::NotifyHit(myComponent, other, otherComp, selfMoved, hitLocation, hitNormal, normalImpulse, hitResult);
		InContact = true;
	}

	// Is the ball bearing in contact with any other geometry?
	bool InContact = false;


private:

	// The initial location of the ball bearing at game start.
	FVector InitialLocation = FVector::ZeroVector;

	// Allow the ball bearing HUD unfettered access to this class.
	friend class ABallBearingHUD;

};
