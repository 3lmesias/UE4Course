// Fill out your copyright notice in the Description page of Project Settings.


#include "BallBearingHUD.h"
#include "PlayerBallBearing.h"

void ABallBearingHUD::DrawHUD()
{
	Super::DrawHUD();

	APlayerBallBearing* ballBearing = Cast<APlayerBallBearing>(GetOwningPawn());

	if (ballBearing != nullptr)
	{
		AddFloat(L"Speed", ballBearing->GetVelocity().Size() / 100.0f);
		AddFloat(L"Dash timer", ballBearing->DashTimer);
		AddFloat(L"Input latitude", ballBearing->InputLatitude);
		AddFloat(L"Input longitude", ballBearing->InputLongitude);
	}
}