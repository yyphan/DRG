// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalBase.h"
#include "PortalManager.h"
#include "Components/DecalComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortalBase::APortalBase()
{
	// Create Box Collision
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);
	BoxCollision->BodyInstance.SetCollisionProfileName("Portal");
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APortalBase::OnBeginOverlap);
	
 	// Create Capture Component
	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponent"));
	CaptureComponent->SetupAttachment(BoxCollision);
	
	// Create Projection
	Projection = CreateDefaultSubobject<UDecalComponent>(TEXT("Projection"));
	Projection->SetupAttachment(BoxCollision);
}

void APortalBase::OnBeginOverlap(class UPrimitiveComponent* OverlappedCom, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PortalManager != nullptr && PortalManager->GetCanTeleport())
	PortalManager->TryTeleport(OtherActor, this);
}

void APortalBase::SetPortalManager(UPortalManager* Manager)
{
	PortalManager = Manager;
}

