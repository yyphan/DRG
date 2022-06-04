// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../TP_WeaponComponent.h"
#include "../Enum.h"
#include "TP_GrappleGunWeaponComponent.generated.h"

class UCableComponent;
class AHookProjectile;

/**
 *	Grapple Gun: Hold LMB to swing, hold RMB to pull close
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRG_API UTP_GrappleGunWeaponComponent : public UTP_WeaponComponent
{
	GENERATED_BODY()

public:
	UTP_GrappleGunWeaponComponent();
	
	/** Attaches the actor to a FirstPersonCharacter */
	void AttachWeapon(ADRGCharacter* TargetCharacter) override;

	/** Detaches the actor to a FirstPersonCharacter */
	void DetachWeapon(ADRGCharacter* TargetCharacter) override;

	/** Make the weapon fires grapple hook on left click */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void OnLeftClick();
	
	/** Release left click to reset the grapple hook */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void OnReleaseLeftClick();

	/** Make the weapon fires pulling cable on right click */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void OnRightClick();

	/** Release right click to release the cable */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void OnReleaseRightClick();

	/** Hook Projectile class to fire */
	UPROPERTY(EditAnywhere, Category="Grapple")
	TSubclassOf<AHookProjectile> HookProjetileClass;

	UPROPERTY(EditAnywhere, Category="Grapple")
	float SwingSpeedFactor = 3;

	UPROPERTY(EditAnywhere, Category="Grapple")
	float PullSpeed = 1600;
	
protected:
	void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindEvents() const;

	void UnBindEvents() const;

	AActor* SpawnHook(EGrappleType GrappleType);

	void ResetCable();
	
	/** The actual cable shooting from the gun */
	UCableComponent* CableComponent;

	/** Reference to the hook point */
	AActor* HookPointActor;

	bool bIsLeftClickGrappleConnected;

	void SetIsLeftClickGrappleConnected();

	void UnsetIsLeftClickGrappleConnected();

	bool bIsRightClickGrappleConnected;

	void SetIsRightClickGrappleConnected();

	void UnsetIsRightClickGrappleConnected();

	void ApplySwingForce() const;

	void ApplyPullingForce() const;
};
