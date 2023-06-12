// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"
#include "DebugHUD.generated.h"

/**
 * 
 */
UCLASS()
class ADebugHUD : public AHUD
{
	GENERATED_BODY()

protected:
	ADebugHUD();

	void AddText(const TCHAR* title, const FText& value)
	{
		RenderStatistic(title, value);
	}

	void AddFloat(const TCHAR* title, float value)
	{
		RenderStatistic(title, FText::AsNumber(value));
	}

	void AddInt(const TCHAR* title, int32 value)
	{
		RenderStatistic(title, FText::AsNumber(value));
	}

	void AddBool(const TCHAR* title, bool value)
	{
		RenderStatistic(title, BoolToText(value), (value == false) ? FLinearColor::Red : FLinearColor::Green);
	}

	// Draw the HUD.
	virtual void DrawHUD() override
	{
		X = Y = 50.0f;
	}

	// The horizontal offset to render the statistic values at.
	float HorizontalOffset = 150.0f;


private:
	// Font used to render the debug information.
	UPROPERTY(Transient)
		UFont* MainFont = nullptr;

	FText CStringToText(const TCHAR* text)
	{
		return FText::FromString(text);
	}

	FText BoolToText(bool value)
	{
		return CStringToText((value == true) ? TEXT("true") : TEXT("false"));
	}

	void RenderStatistic(const TCHAR* title, const FText& value, const FLinearColor& valueColor = FLinearColor::White)
	{
		FCanvasTextItem item0(FVector2D(X, Y), CStringToText(title), MainFont, FLinearColor::White);
		item0.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
		Canvas->DrawItem(item0);
		FCanvasTextItem item1(FVector2D(X + HorizontalOffset, Y), value, MainFont, valueColor);
		item1.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
		Canvas->DrawItem(item1);
		Y += LineHeight;
	}

	// The current X coordinate.
	float X = 50.0f;

	// The current Y coordinate.
	float Y = 50.0f;

	// The line height to separate each HUD entry.
	float LineHeight = 16.0f;
};
