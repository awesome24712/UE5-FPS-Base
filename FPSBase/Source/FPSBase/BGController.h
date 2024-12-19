// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BGController.generated.h"

class AUIConductor;
class ABGPlayer;


/**
 * 
 */
UCLASS()
class FPSBASE_API ABGController : public APlayerController
{
	GENERATED_BODY()

public:
	ABGController();

	void BeginPlay() override;

	void PlayerTick(float deltaTime) override;

	void SetupInputComponent() override; //for non-gameplay inputs

	//Sends non-interactive blurb to user
	void NotifyQuick() {}

	//Sends pop-up box to user
	void NotifyBox() {}

	void SetupUIConductor();

//------------------------------------------------
// Getters
//------------------------------------------------

	bool IsInMenu() const;

	bool IsTyping() const;

	FVector2D GetCursorPos() const;

	inline TObjectPtr<AUIConductor> GetUIC() const { return m_uiConductor; }

	ABGPlayer* GetPlayerCharacter();

	bool WantsCursor() const { return false; }

//------------------------------------------------
// Input events
//------------------------------------------------
	void OnToggleMainMenu();

	void OnToggleTeamMenu();

	void OnToggleClassMenu();

	void OnToggleAdminMenu();

	void OnToggleCommMenu1();

	void OnToggleCommMenu2();

	void OnToggleCommMenu3();

	void OnToggleScoreboard();

	void OnEnterMapMaker();

private:

	TObjectPtr<AUIConductor> m_uiConductor;

};
