// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_PortalGunWeaponComponent.h"
#include "PortalManager.h"
#include "../DRGCharacter.h"

void UTP_PortalGunWeaponComponent::AttachWeapon(ADRGCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if(Character != nullptr)
	{
		// Attach the weapon to the First Person Character
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(Character->GetMesh1P(),AttachmentRules, FName(TEXT("GripPoint")));

		// Register left click event
		Character->OnLeftClick.AddDynamic(this, &UTP_PortalGunWeaponComponent::FireLeftClick);
		// Register right click event
		Character->OnRightClick.AddDynamic(this, &UTP_PortalGunWeaponComponent::FireRightClick);
	}
}

void UTP_PortalGunWeaponComponent::DetachWeapon(ADRGCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character != nullptr)
	{
		// Weapon will be hidden at World Position 0,0,0
		const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, true);
		GetOwner()->DetachFromActor(DetachmentRules);

		// Unregister all events
		Character->OnLeftClick.RemoveAll(this);
		Character->OnRightClick.RemoveAll(this);
	}
}

void UTP_PortalGunWeaponComponent::FireLeftClick()
{
	PortalManager->CreatePortal(EPortalType::LeftClickPortal);
}

void UTP_PortalGunWeaponComponent::FireRightClick()
{
	PortalManager->CreatePortal(EPortalType::RightClickPortal);
}

void UTP_PortalGunWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	// Get reference of Portal Manager
	PortalManager = Cast<UPortalManager>(GetOwner()->GetComponentByClass(UPortalManager::StaticClass()));
}

void UTP_PortalGunWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Character != nullptr)
	{
		// Unregister all events
		Character->OnLeftClick.RemoveDynamic(this, &UTP_PortalGunWeaponComponent::FireLeftClick);
		Character->OnRightClick.RemoveDynamic(this, &UTP_PortalGunWeaponComponent::FireRightClick);
	}
}
