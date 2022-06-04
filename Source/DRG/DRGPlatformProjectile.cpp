// Fill out your copyright notice in the Description page of Project Settings.


#include "DRGPlatformProjectile.h"



void ADRGPlatformProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		// Spawn a platform
		const FVector Location = GetActorLocation();
		const FRotator Rotation = FRotator(0,0,0);
		GetWorld()->SpawnActor(PlatformClass, &Location, &Rotation);

		Destroy();
	}
}