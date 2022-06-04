// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PortalBase.generated.h"

class UPortalManager;

UCLASS(Blueprintable)
class DRG_API APortalBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalBase();

	// Root Component, detects collision
	UPROPERTY(VisibleAnywhere, Category="Portal", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision;

	// Responsible of TAKING snapshot and OUTPUT to a render texture
	UPROPERTY(VisibleAnywhere, Category="Portal", meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* CaptureComponent;
	
	// Responsible of displaying the portal
	UPROPERTY(VisibleAnywhere, Category="Portal", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* Projection;

protected:
	// Reference to the Portal Manager that had created this portal
	UPortalManager* PortalManager;

	// Upon colliding with a Pawn (in this case, player)
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedCom, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Used to set up the reference
	void SetPortalManager(UPortalManager* Manager);
};
