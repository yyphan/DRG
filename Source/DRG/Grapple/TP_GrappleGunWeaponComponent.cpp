// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_GrappleGunWeaponComponent.h"
#include "CableComponent.h"
#include "HookProjectile.h"
#include "../DRGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UTP_GrappleGunWeaponComponent::UTP_GrappleGunWeaponComponent()
{
	MuzzleOffset = FVector(200, 0, 10);
	UnsetIsLeftClickGrappleConnected();
	UnsetIsRightClickGrappleConnected();

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UTP_GrappleGunWeaponComponent::AttachWeapon(ADRGCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if(Character != nullptr)
	{
		// Attach the weapon to the First Person Character
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(Character->GetMesh1P(),AttachmentRules, FName(TEXT("GripPoint")));

		BindEvents();
	}
}

void UTP_GrappleGunWeaponComponent::DetachWeapon(ADRGCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character != nullptr)
	{
		// Weapon will be hidden at World Position 0,0,0
		const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, true);
		GetOwner()->DetachFromActor(DetachmentRules);

		UnBindEvents();

		ResetCable();
	}
}

void UTP_GrappleGunWeaponComponent::OnLeftClick()
{
	ResetCable();

	// Spawn a Hook Projectile and use as Hook Point
	HookPointActor = SpawnHook(EGrappleType::LeftClickGrapple);

	if (HookPointActor == nullptr || CableComponent == nullptr) return;

	// Attach the end of the cable to the Hook Point
	CableComponent->SetAttachEndTo(HookPointActor, EName::None, EName::None);
	CableComponent->SetHiddenInGame(false);
}

void UTP_GrappleGunWeaponComponent::OnReleaseLeftClick()
{
	ResetCable();
}

void UTP_GrappleGunWeaponComponent::OnRightClick()
{
	ResetCable();

	// Spawn a Hook Projectile and use as Hook Point
	HookPointActor = SpawnHook(EGrappleType::RightClickGrapple);

	if (HookPointActor == nullptr || CableComponent == nullptr) return;

	// Attach the end of the cable to the Hook Point
	CableComponent->SetAttachEndTo(HookPointActor, EName::None, EName::None);
	CableComponent->SetHiddenInGame(false);
}

void UTP_GrappleGunWeaponComponent::OnReleaseRightClick()
{
	ResetCable();
}

void UTP_GrappleGunWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	CableComponent = Cast<UCableComponent>(GetOwner()->GetComponentByClass(UCableComponent::StaticClass()));
	if (CableComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CableCompnent not found"));
	}
}

void UTP_GrappleGunWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnBindEvents();
}

void UTP_GrappleGunWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsLeftClickGrappleConnected)
	{
		ApplySwingForce();
	}
	else if (bIsRightClickGrappleConnected)
	{
		ApplyPullingForce();
	}
}

void UTP_GrappleGunWeaponComponent::BindEvents() const
{
	if(Character != nullptr)
	{
		// Register left click events
		Character->OnLeftClick.AddDynamic(this, &UTP_GrappleGunWeaponComponent::OnLeftClick);
		Character->OnReleaseLeftClick.AddDynamic(this, &UTP_GrappleGunWeaponComponent::OnReleaseLeftClick);
		// Register right click event
		Character->OnRightClick.AddDynamic(this, &UTP_GrappleGunWeaponComponent::OnRightClick);
		Character->OnReleaseRightClick.AddDynamic(this, &UTP_GrappleGunWeaponComponent::OnReleaseRightClick);
	}
}


void UTP_GrappleGunWeaponComponent::UnBindEvents() const
{
	if(Character != nullptr)
	{
		// Register left click events
		Character->OnLeftClick.RemoveDynamic(this, &UTP_GrappleGunWeaponComponent::OnLeftClick);
		Character->OnReleaseLeftClick.RemoveDynamic(this, &UTP_GrappleGunWeaponComponent::OnReleaseLeftClick);
		// Register right click event
		Character->OnRightClick.RemoveDynamic(this, &UTP_GrappleGunWeaponComponent::OnRightClick);
		Character->OnReleaseRightClick.RemoveDynamic(this, &UTP_GrappleGunWeaponComponent::OnReleaseRightClick);
	}
}

AActor* UTP_GrappleGunWeaponComponent::SpawnHook(EGrappleType GrappleType)
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnLocation = PlayerController->PlayerCameraManager->GetCameraLocation() + SpawnRotation.RotateVector(MuzzleOffset);;

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	AHookProjectile* Hook = GetWorld()->SpawnActor<AHookProjectile>(HookProjetileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	switch (GrappleType)
	{
	case EGrappleType::LeftClickGrapple:
		Hook->OnHookedDelegate.BindUObject(this, &UTP_GrappleGunWeaponComponent::SetIsLeftClickGrappleConnected);
		break;
	case EGrappleType::RightClickGrapple:
		Hook->OnHookedDelegate.BindUObject(this, &UTP_GrappleGunWeaponComponent::SetIsRightClickGrappleConnected);
		break;
	}
	
	return Hook;
}

void UTP_GrappleGunWeaponComponent::ResetCable()
{
	// Remove Hook Point
	if (HookPointActor != nullptr)
	{
		HookPointActor->Destroy();
		HookPointActor = nullptr;
	}
	
	// Hide Cable
	if (CableComponent != nullptr)
	{
		CableComponent->SetAttachEndTo(GetOwner(), EName::None, EName::None);
		CableComponent->SetHiddenInGame(false);
	}

	UnsetIsLeftClickGrappleConnected();
	UnsetIsRightClickGrappleConnected();
}

void UTP_GrappleGunWeaponComponent::SetIsLeftClickGrappleConnected()
{
	bIsLeftClickGrappleConnected = true;
}

void UTP_GrappleGunWeaponComponent::UnsetIsLeftClickGrappleConnected()
{
	bIsLeftClickGrappleConnected = false;
}

void UTP_GrappleGunWeaponComponent::SetIsRightClickGrappleConnected()
{
	bIsRightClickGrappleConnected = true;
}

void UTP_GrappleGunWeaponComponent::UnsetIsRightClickGrappleConnected()
{
	bIsRightClickGrappleConnected = false;
}

void UTP_GrappleGunWeaponComponent::ApplySwingForce() const
{
	// Cable force direction is from gun to hook point
	const FVector GrappleGunLocation = GetOwner()->GetActorLocation();
	const FVector HookPointLocation = HookPointActor->GetActorLocation();
	FVector CableForceVector = HookPointLocation - GrappleGunLocation;

	// Aim is to change the direction of current velocity to perpendicular to the cable
	// Use the inversed dot product of the cable force and the current velocity
	// When velocity and cable force finally be perpendicular, cable force will be 0
	const ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter == nullptr) return;
	const FVector Velocity = PlayerCharacter->GetVelocity();
	// Invert the dot product, better understood by drawing the vectors
	const float CableForceVectorLength = -1 * FVector::DotProduct(CableForceVector, Velocity);
	
	// Cable force unit vector
	CableForceVector.Normalize(1);

	// Final cable force
	const FVector CableForce = CableForceVector * CableForceVectorLength * SwingSpeedFactor;

	PlayerCharacter->GetCharacterMovement()->AddForce(CableForce);
}

void UTP_GrappleGunWeaponComponent::ApplyPullingForce() const
{
	// Cable force direction is from gun to hook point
	const FVector GrappleGunLocation = GetOwner()->GetActorLocation();
	const FVector HookPointLocation = HookPointActor->GetActorLocation();
	FVector CableForceVector = HookPointLocation - GrappleGunLocation;

	const ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter == nullptr) return;
	
	// Cable force unit vector
	CableForceVector.Normalize(1);

	// Final cable force
	const FVector CableForce = CableForceVector * PullSpeed;

	PlayerCharacter->GetCharacterMovement()->Launch(CableForce);
}
