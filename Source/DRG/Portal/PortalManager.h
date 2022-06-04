// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enum.h"
#include "Components/ActorComponent.h"
#include "PortalManager.generated.h"

class APortalBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DRG_API UPortalManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPortalManager();

	// BP portals to be spawned
	UPROPERTY(EditAnywhere, Category="Portal")
	TSubclassOf<APortalBase> LeftClickPortal;
	UPROPERTY(EditAnywhere, Category="Portal")
	TSubclassOf<APortalBase> RightClickPortal;

	// References to existing portals
	APortalBase* CurrentLeftClickPortal;
	APortalBase* CurrentRightClickPortal;
	
protected:
	// Used in FTimerManager::SetTimer
	FTimerHandle PortalTimerHandle;

	// The time the portals have to wait after teleporting, before doing it again
	float TeleportCooldown;

	// Determines whether the portals are ready to teleport
	bool bCanTeleport;

public:
	// Spawns a portal on a surface
    void CreatePortal(EPortalType PortalType);
	
	void TryTeleport(AActor* Pawn, APortalBase* Entrance);

	// Reset the status after cooldown, signifying a new teleport is possible
	void ResetTeleportStatus();

	bool GetCanTeleport() const;
};
