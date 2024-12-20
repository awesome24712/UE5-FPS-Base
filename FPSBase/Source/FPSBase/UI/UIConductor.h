// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UIConductor.generated.h"

class ABGPlayer;

/**
 * 
 */
UCLASS()
class FPSBASE_API AUIConductor : public AHUD
{
	GENERATED_BODY()
	
public:
	AUIConductor();

	void Setup();

	void ShowHUD() override;

	void PostInitializeComponents() override;

	/************************************************************************************************************
	 Actor Render - These functions allow for actors in the world to gain access to the hud and render
	 information on it.
	************************************************************************************************************/
	/** remove an actor from the PostRenderedActors array */
	void RemovePostRenderedActor(AActor* A) override;

	/** add an actor to the PostRenderedActors array */
	void AddPostRenderedActor(AActor* A) override;

	/** PostRender is the main draw loop. */
	void PostRender() override;

	/** The Main Draw loop for the hud.  Gets called before any messaging.  Should be subclassed */
	void DrawHUD() override;

	/**
	 *	Pauses or unpauses the game due to main window's focus being lost.
	 *	@param Enable - tells whether to enable or disable the pause state
	 */
	void OnLostFocusPause(bool bEnable) override;


	/** 
	* Getters
	* 
	*/
	bool WantsCursor() const;

	ABGPlayer* GetPlayer() const;

	int			ScreenHeight() const;
	int			ScreenWidth() const;
	FIntPoint	ScreenSize() const;
};
