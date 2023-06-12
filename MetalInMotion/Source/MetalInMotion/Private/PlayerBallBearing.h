// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "BallBearing.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerBallBearing.generated.h"

/**
 * 
 */
UCLASS()
class APlayerBallBearing : public ABallBearing
{
	GENERATED_BODY()

public:
	// sets defaults
	APlayerBallBearing();

	//spring arm for positioning camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BallBearing)
		USpringArmComponent* SpringArm = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BallBearing)
		UCameraComponent* Camera = nullptr;
	//Force to push de ball around
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BallBearing)
		float ControllerForce = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BallBearing)
		float JumpForce = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BallBearing)
		float DashForce = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BallBearing)
		float MaximumSpeed = 4.0f;

protected:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// The current longitude input received from the player.
	float InputLongitude = 0.0f;

	// The current latitude input received from the player.
	float InputLatitude = 0.0f;

	// Timer used to control the dashing of the ball bearing.
	float DashTimer = 0.0f;

	// move in x
	void MoveLongitudinally(float value) {
		InputLongitude = value;
	}
	// move in y
	void MoveLaterally(float value) {
		InputLatitude = value;
	}

	void Jump();

	// Have the ball bearing perform a dash.
	void Dash();

	friend class ABallBearingHUD;
};
