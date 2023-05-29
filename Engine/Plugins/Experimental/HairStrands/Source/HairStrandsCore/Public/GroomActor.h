// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
#include "GroomActor.generated.h"


/** An actor that renders a simulated hair */
UCLASS(HideCategories = (Input, Replication, Mobility), showcategories = ("Input|MouseInput", "Input|TouchInput"))
class HAIRSTRANDSCORE_API AGroomActor : public AActor
{
	GENERATED_UCLASS_BODY()

	/** Strand hair component that performs simulation and rendering */
	UPROPERTY(Category = StrandHair, VisibleAnywhere, BlueprintReadOnly)
	class UGroomComponent* GroomComponent;

#if WITH_EDITORONLY_DATA
protected:
	/** Billboard used to see the scene in the editor */
	UPROPERTY()
		UBillboardComponent* SpriteComponent;
#endif

public:
	/** Returns GroomComponent subobject **/
	class UGroomComponent* GetGroomComponent() const { return GroomComponent; }
};
