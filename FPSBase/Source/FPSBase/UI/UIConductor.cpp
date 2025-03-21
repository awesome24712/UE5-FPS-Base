// Fill out your copyright notice in the Description page of Project Settings.


#include "UIConductor.h"
#include "UIHelpers.h"
#include "../BGPlayer.h"
#include "../BGController.h"
#include "Crosshair/Crosshair.h"
#include "AwesomeGlass.h"

using namespace UIHelpers;

AUIConductor::AUIConductor() {
	m_flUiScale = 1.0f;
}


void AUIConductor::Setup() {
	if (!GEngine) {
		return;
	}

	FIntPoint screenSize = GEngine->GameViewport->Viewport->GetSizeXY();
	m_flHeight = screenSize.Y;
	m_flWidth = screenSize.X;

	Crosshair::LoadCrosshairSettings();
	SharedAssets::LoadSharedAssets();

	if (!m_hud) {
		m_hud = new AwesomeHUD();
		m_hud->LoadAssets();
	}

	PerformLayout();
}

void AUIConductor::ShowHUD() {
	Super::ShowHUD();
}

void AUIConductor::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

void AUIConductor::RemovePostRenderedActor(AActor* A) {
	Super::RemovePostRenderedActor(A);
}

void AUIConductor::AddPostRenderedActor(AActor* A) {
	Super::AddPostRenderedActor(A);
}

void AUIConductor::PostRender() {
	Super::PostRender();

	auto pPlayer = GetPlayer();
	if (!GEngine || !pPlayer) {
		return;
	}


	FString v = GetOwningPawn() ? UIHelpers::VectorToString(GetOwningPawn()->GetVelocity()) : "MISSING PAWN";
	DrawText(LangSystem::Localize("#ShowPos"), FColor::White, ScreenWidth() - 200, 10);
	DrawText(v, FColor::White, ScreenWidth() - 200, 30);

	FString r = UIHelpers::RotatorToString(GetController()->PlayerCameraManager->GetCameraRotation());
	DrawText(r, FColor::White, ScreenWidth() - 200, 50);
	
	//DrawRect(FColor::White, ScreenWidth() - 200, 30, 30, 30);

	//c->DrawRect(FColor(255, 0, 0, 128), loc.X, loc.Y, size.X, size.Y);

	Crosshair::DrawCrosshair(this, pPlayer);
	m_hud->Draw(this);
}

void AUIConductor::DrawHUD() {
	Super::DrawHUD();

	//do NOT access canvas from here
}

/**
*	Pauses or unpauses the game due to main window's focus being lost.
*	@param Enable - tells whether to enable or disable the pause state
*/
void AUIConductor::OnLostFocusPause(bool bEnable) {
	Super::OnLostFocusPause(bEnable);
}

void AUIConductor::PerformLayout() {
	m_hud->PerformLayout(this);
}

bool AUIConductor::WantsCursor() const {
	return false;
}

ABGPlayer* AUIConductor::GetPlayer() const {
	return  PlayerOwner->GetPawn<ABGPlayer>();
}

APlayerController* AUIConductor::GetController() const
{
	return Cast<APlayerController>(GetPlayer()->Controller);;
}

FIntPoint AUIConductor::ScreenSize() const {
	return GEngine->GameViewport->Viewport->GetSizeXY();
}
