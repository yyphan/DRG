// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HookProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

DECLARE_DELEGATE(FOnHookedDelegate);
/**
 * Projectile for the Grapple Gun
 */
UCLASS()
class DRG_API AHookProjectile : public AActor
{
	GENERATED_BODY()

private:
	/** Cube collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UBoxComponent* CollisionComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	AHookProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FOnHookedDelegate OnHookedDelegate;
};
