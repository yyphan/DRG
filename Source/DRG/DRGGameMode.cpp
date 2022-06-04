// Copyright Epic Games, Inc. All Rights Reserved.

#include "DRGGameMode.h"
#include "DRGCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADRGGameMode::ADRGGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
