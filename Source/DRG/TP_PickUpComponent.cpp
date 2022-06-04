// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PickUpComponent.h"

#include "TP_WeaponComponent.h"

UTP_PickUpComponent::UTP_PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UTP_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);
}

void UTP_PickUpComponent::OnPickUpBy(ADRGCharacter* PickUpCharacter) const
{
	UTP_WeaponComponent* WeaponComponent = Cast<UTP_WeaponComponent>(GetOwner()->GetComponentByClass(UTP_WeaponComponent::StaticClass()));
	PickUpCharacter->AddWeapon(WeaponComponent);
}

void UTP_PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ADRGCharacter* Character = Cast<ADRGCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// The actor is being picked up
		OnPickUpBy(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
