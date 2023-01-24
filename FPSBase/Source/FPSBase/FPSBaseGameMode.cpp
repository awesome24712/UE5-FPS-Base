// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSBaseGameMode.h"
#include "FPSBaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSBaseGameMode::AFPSBaseGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
