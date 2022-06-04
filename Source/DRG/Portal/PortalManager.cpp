// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"
#include "PortalBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPortalManager::UPortalManager()
{
	TeleportCooldown = 0.2;
	bCanTeleport = true;
}

void UPortalManager::CreatePortal(EPortalType PortalType)
{
	// Prepare for line trace
	FHitResult OutHit;
	// Using player camera for line trace
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	const FVector Start = PlayerController->PlayerCameraManager->GetCameraLocation();
	const FVector ForwardActor = PlayerController->PlayerCameraManager->GetActorForwardVector();
	const FVector End = (Start + (ForwardActor * 10000.f));
	FCollisionQueryParams LineTraceParams = FCollisionQueryParams(FName(TEXT("PortalLineTrace")), false, GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, LineTraceParams))
	{
		// The facing direction of the surface at the point of impact
		const FRotator SurfaceRotation = OutHit.ImpactNormal.Rotation();
		// Spawn the portal at the point of impact, with the facing direction of the surface
		switch(PortalType)
		{
		case EPortalType::LeftClickPortal:
			// Destroy previous instance of LeftClickPortal
			if (CurrentLeftClickPortal != nullptr) CurrentLeftClickPortal->Destroy();
			// Spawn LeftClickPortal
			CurrentLeftClickPortal = GetWorld()->SpawnActor<APortalBase>(LeftClickPortal, OutHit.Location, SurfaceRotation);
			// Set up reference
			CurrentLeftClickPortal->SetPortalManager(this);
			break;
		case EPortalType::RightClickPortal:
			// Destroy previous instance of RightClickPortal
			if (CurrentRightClickPortal != nullptr) CurrentRightClickPortal->Destroy();
			// Spawn RightClickPortal
			CurrentRightClickPortal = GetWorld()->SpawnActor<APortalBase>(RightClickPortal, OutHit.Location, SurfaceRotation);
			// Set up reference
			CurrentRightClickPortal->SetPortalManager(this);
			break;
		}
	}
}

void UPortalManager::TryTeleport(AActor* Pawn, APortalBase* Entrance)
{
	// Making sure both portals were set
	if (CurrentLeftClickPortal == nullptr || CurrentRightClickPortal == nullptr) return;
	// Making sure teleport is ready
	if (!bCanTeleport) return;

	// Not available for new teleports until some time later
	bCanTeleport = false;

	// Determines the Exit, which is the portal other than Entrance
	APortalBase* Exit = CurrentRightClickPortal;
	if (Entrance == CurrentRightClickPortal)
	{
		Exit = CurrentLeftClickPortal;
	}

	// Teleport the pawn to Exit, but a little further than the portal itself
	const FVector ExitPortalLocation = Exit->GetActorLocation();
	const FVector ExitFacingVector = Exit->GetActorForwardVector();
	const FVector TeleportEndLocation = ExitPortalLocation + ExitFacingVector * 200;
	Pawn->SetActorLocation(TeleportEndLocation);
	
	// Facing the direction of coming out of Exit
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController != nullptr)
	{
		// Vector Math
		const FVector PlayerFacingVector = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
		const FVector EntranceFacingVector = Entrance->GetActorForwardVector();
		const FVector LocalExitAngleVector = PlayerFacingVector + EntranceFacingVector;
		FVector WorldExitAngle = ExitFacingVector - LocalExitAngleVector;
		// NEVER facing towards Z axis (always remain standing)
		WorldExitAngle.Z = PlayerFacingVector.Z;

		// End Rotation: Like going through a door
		const FRotator PlayerEndRotation = WorldExitAngle.Rotation();
		PlayerController->SetControlRotation(PlayerEndRotation);
	}
	
	// Set velocity when coming out of Exit
	const float EntranceVelocityLength = Pawn->GetVelocity().Length();
	const FVector ExitForwardVector = Exit->GetActorForwardVector();
	const FVector ExitVelocity = EntranceVelocityLength * ExitForwardVector;
	ACharacter* PlayerCharacter = Cast<ACharacter>(Pawn);
	PlayerCharacter->LaunchCharacter(ExitVelocity, true, true);

	// Set timer to reset teleport status after a cooldown period
	GetOwner()->GetWorldTimerManager().SetTimer(PortalTimerHandle, this, &UPortalManager::ResetTeleportStatus, TeleportCooldown, false);
}

void UPortalManager::ResetTeleportStatus()
{
	bCanTeleport = true;
}

bool UPortalManager::GetCanTeleport() const
{
	return bCanTeleport;
}

