// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UIConductor.generated.h"

class ABGPlayer;
class AwesomeHUD;

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

	void PerformLayout(); //calls PerformLayout on all top-level AwesomeGlass components


	/** 
	* Getters
	* 
	*/
	bool WantsCursor() const;

	ABGPlayer* GetPlayer() const;
	APlayerController* GetController() const;

	float		ScreenHeight() const { return m_flHeight; }
	float		ScreenWidth() const { return m_flWidth; }
	FIntPoint	ScreenSize() const;
	float		GetScale() const { return m_flUiScale;  }

	/**
	 * Variables
	 */
private:
	float m_flUiScale;

	float m_flHeight;
	float m_flWidth;

	//Top-level AwesomeGlass components
	AwesomeHUD* m_hud;
};

//assumes GEngine is non-null
//inline AUIConductor* GetLocalConductor() {
//	return GEngine->GetFirstLocalPlayerController(GEngine->GetWorld())
//}
