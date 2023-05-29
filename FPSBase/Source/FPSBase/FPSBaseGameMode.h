// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSBaseGameMode.generated.h"

UCLASS(minimalapi)
class AFPSBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSBaseGameMode();

	/**
	 * Initialize the game.
	 * The GameMode's InitGame() event is called before any other functions (including PreInitializeComponents() )
	 * and is used by the GameMode to initialize parameters and spawn its helper classes.
	 * @warning: this is called before actors' PreInitializeComponents.
	 */
	void InitGame(const FString& mapName, const FString& options, FString& errorMessage) override;

	/**
	 * Initialize the GameState actor with default settings
	 * called during PreInitializeComponents() of the GameMode after a GameState has been spawned
	 * as well as during Reset()
	 */
	void InitGameState() override;

	/**
	 * Allows game to send network messages to provide more information to the client joining the game via NMT_GameSpecific
	 * (for example required DLC)
	 * the out string RedirectURL is built in and send automatically if only a simple URL is needed
	 */
	void GameWelcomePlayer(UNetConnection* connection, FString& redirectURL) override;


	/**
	 * Accept or reject a player attempting to join the server.  Fails login if you set the ErrorMessage to a non-empty string.
	 * PreLogin is called before Login.  Significant game time may pass before Login is called
	 *
	 * @param	Options					The URL options (e.g. name/spectator) the player has passed
	 * @param	Address					The network address of the player
	 * @param	UniqueId				The unique id the player has passed to the server
	 * @param	ErrorMessage			When set to a non-empty value, the player will be rejected using the error message set
	 */
	void PreLogin(const FString& options, const FString& address, const FUniqueNetIdRepl& uniqueId, FString& errorMessage) override;

	/**
	 * Called to login new players by creating a player controller, overridable by the game
	 *
	 * Sets up basic properties of the player (name, unique id, registers with backend, etc) and should not be used to do
	 * more complicated game logic.  The player controller is not fully initialized within this function as far as networking is concerned.
	 * Save "game logic" for PostLogin which is called shortly afterward.
	 *
	 * @param NewPlayer pointer to the UPlayer object that represents this player (either local or remote)
	 * @param RemoteRole the remote role this controller has
	 * @param Portal desired portal location specified by the client
	 * @param Options game options passed in by the client at login
	 * @param UniqueId platform specific unique identifier for the logging in player
	 * @param ErrorMessage [out] error message, if any, why this login will be failing
	 *
	 * If login is successful, returns a new PlayerController to associate with this player. Login fails if ErrorMessage string is set.
	 *
	 * @return a new player controller for the logged in player, NULL if login failed for any reason
	 */
	APlayerController* Login(UPlayer* newPlayer, ENetRole inRemoteRole, const FString& portal, const FString& options, const FUniqueNetIdRepl& uniqueId, FString& errorMessage) override;

	/** Called after a successful login.  This is the first place it is safe to call replicated functions on the PlayerController. */
	void PostLogin(APlayerController* newPlayer) override;

	/** Called when a Controller with a PlayerState leaves the game or is destroyed */
	void Logout(AController* exiting) override;

	/**
	 * Spawns the appropriate PlayerController for the given options; split out from Login() for easier overriding.
	 * Override this to conditionally spawn specialized PlayerControllers, for instance.
	 *
	 * @param RemoteRole the role this controller will play remotely
	 * @param Options the options string from the new player's URL
	 *
	 * @return PlayerController for the player, NULL if there is any reason this player shouldn't exist or due to some error
	 */
	APlayerController* SpawnPlayerController(ENetRole inRemoteRole, const FString& options) override;

	/**
	 * Sets the name for a controller
	 * @param Controller	The controller of the player to change the name of
	 * @param NewName		The name to set the player to
	 * @param bNameChange	Whether the name is changing or if this is the first time it has been set
	 */
	void ChangeName(AController* controller, const FString& newName, bool bNameChange) override;

	/**
	 * Used to notify the game type that it is ok to update a player's gameplay
	 * specific muting information now. The playercontroller needs to notify
	 * the server when it is possible to do so or the unique net id will be
	 * incorrect and the muting not work.
	 *
	 * @param aPlayer the playercontroller that is ready for updates
	 */
	void UpdateGameplayMuteList(APlayerController* aPlayer);

	/**
	 * Customize incoming player based on URL options
	 *
	 * @param NewPlayerController player logging in
	 * @param UniqueId unique id for this player
	 * @param Options URL options that came at login
	 *
	 */
	FString InitNewPlayer(APlayerController* newPlayerController, const FUniqueNetIdRepl& uniqueId, const FString& options, const FString& portal = TEXT("")) override;

	/** Initialize the AHUD object for a player. Games can override this to do something different */
	void InitializeHUDForPlayer(APlayerController* newPlayer);

	/**
	 * Called in the event that we fail to spawn a controller's pawn, which maybe it didn't have one or maybe it tried
	 * to spawn and was destroyed due to collision.
	 */
	void FailedToRestartPlayer(AController* newPlayer) override;
};



