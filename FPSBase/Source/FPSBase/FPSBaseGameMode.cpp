// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSBaseGameMode.h"
#include "BGPlayer.h"
#include "UObject/ConstructorHelpers.h"
#include "BGController.h"
#include "UI/UIConductor.h"
#include "Systems/SystemManager.h"
#include "FPSBasePlayerState.h"

UWorld* g_pCurrentWorld;

AFPSBaseGameMode::AFPSBaseGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	DefaultPawnClass = ABGPlayer::StaticClass();
	DefaultPlayerName = FText::FromString("Unknown Soldier");
	HUDClass = AUIConductor::StaticClass();
	PlayerControllerClass = ABGController::StaticClass();
	PlayerStateClass = AFPSBasePlayerState::StaticClass();
}

void AFPSBaseGameMode::InitGame(const FString& mapName, const FString& options, FString& errorMessage) {
	Super::InitGame(mapName, options, errorMessage);

	g_pSystems->InitializeAllSystems();
}

void AFPSBaseGameMode::InitGameState() {
	Super::InitGameState();
}

void AFPSBaseGameMode::GameWelcomePlayer(UNetConnection* connection, FString& redirectURL) {
	Super::GameWelcomePlayer(connection, redirectURL);
}

void AFPSBaseGameMode::PreLogin(const FString& options, const FString& address, const FUniqueNetIdRepl& uniqueId, FString& errorMessage) {
	Super::PreLogin(options, address, uniqueId, errorMessage);
}

APlayerController* AFPSBaseGameMode::Login(UPlayer* newPlayer, ENetRole inRemoteRole, const FString& portal, const FString& options, const FUniqueNetIdRepl& uniqueId, FString& errorMessage) {
	return Super::Login(newPlayer, inRemoteRole, portal, options, uniqueId, errorMessage);
}

void AFPSBaseGameMode::PostLogin(APlayerController* newPlayer) {
	Super::PostLogin(newPlayer);
}

void AFPSBaseGameMode::Logout(AController* exiting) {
	Super::Logout(exiting);
}

APlayerController* AFPSBaseGameMode::SpawnPlayerController(ENetRole inRemoteRole, const FString& options) {
	return Super::SpawnPlayerController(inRemoteRole, options);
}

void AFPSBaseGameMode::ChangeName(AController* controller, const FString& newName, bool bNameChange) {
	Super::ChangeName(controller, newName, bNameChange);
}

void AFPSBaseGameMode::UpdateGameplayMuteList(APlayerController* aPlayer) {
	Super::UpdateGameplayMuteList(aPlayer);
}

FString AFPSBaseGameMode::InitNewPlayer(APlayerController* newPlayerController, const FUniqueNetIdRepl& uniqueId, const FString& options, const FString& portal) {
	return Super::InitNewPlayer(newPlayerController, uniqueId, options, portal);
}

void AFPSBaseGameMode::InitializeHUDForPlayer(APlayerController* newPlayer) {
	Super::InitializeHUDForPlayer(newPlayer);
}

void AFPSBaseGameMode::FailedToRestartPlayer(AController* newPlayer) {
	Super::FailedToRestartPlayer(newPlayer);
}
