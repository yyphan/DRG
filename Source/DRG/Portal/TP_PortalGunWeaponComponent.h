// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../TP_WeaponComponent.h"
#include "TP_PortalGunWeaponComponent.generated.h"

class UPortalManager;

/**
 * 
 */
UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRG_API UTP_PortalGunWeaponComponent : public UTP_WeaponComponent
{
	GENERATED_BODY()

public:
	/** Attaches the actor to a FirstPersonCharacter */
	void AttachWeapon(ADRGCharacter* TargetCharacter) override;

	/** Detaches the actor to a FirstPersonCharacter */
	void DetachWeapon(ADRGCharacter* TargetCharacter) override;

	/** Make the weapon fires a portal on left click*/
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void FireLeftClick();

	/** Make the weapon fires another (type of) portal on right click*/
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void FireRightClick();

	// Portal Manager is in charge of creating and managing portals
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Portal")
	UPortalManager* PortalManager;
	
protected:
	void BeginPlay() override;
	
	/** Ends gameplay for this component. */
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
