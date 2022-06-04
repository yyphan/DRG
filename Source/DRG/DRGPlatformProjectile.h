// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DRGProjectile.h"
#include "DRGPlatformProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DRG_API ADRGPlatformProjectile : public ADRGProjectile
{
	GENERATED_BODY()
public:
	/** Platform class to spawn */
	UPROPERTY(EditAnywhere, Category=Projectile)
	TSubclassOf<class AActor> PlatformClass;

public:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
