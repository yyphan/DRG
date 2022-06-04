// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "DRGCharacter.h"
#include "TP_PickUpComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DRG_API UTP_PickUpComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UTP_PickUpComponent();
	
protected:

	/** Called when the game starts */
	virtual void BeginPlay() override;

	void OnPickUpBy(ADRGCharacter* PickUpCharacter) const;

	/** Code for when something overlaps this component */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};