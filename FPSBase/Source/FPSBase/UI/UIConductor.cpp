// Fill out your copyright notice in the Description page of Project Settings.


#include "UIConductor.h"
#include "UIHelpers.h"
#include "../BGPlayer.h"
#include "../BGController.h"
#include "Crosshair/Crosshair.h"

using namespace UIHelpers;

AUIConductor::AUIConductor() {

}


void AUIConductor::Setup() {
	//Log("Screensize is %i x %i", ScreenWidth(), ScreenHeight());
	Crosshair::LoadCrosshairSettings();
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

	DrawText(v, FColor::White, ScreenWidth() - 200, 10);
	DrawRect(FColor::White, ScreenWidth() - 200, 30, 30, 30);

	//c->DrawRect(FColor(255, 0, 0, 128), loc.X, loc.Y, size.X, size.Y);

	Crosshair::DrawCrosshair(this, pPlayer);
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

bool AUIConductor::WantsCursor() const {
	return false;
}

ABGPlayer* AUIConductor::GetPlayer() const {
	return  PlayerOwner->GetPawn<ABGPlayer>();
}

int AUIConductor::ScreenHeight() const {
	return (int) GEngine->GameViewport->Viewport->GetSizeXY().Y;
}

int AUIConductor::ScreenWidth() const {
	return (int) GEngine->GameViewport->Viewport->GetSizeXY().X;
}

FIntPoint AUIConductor::ScreenSize() const {
	return GEngine->GameViewport->Viewport->GetSizeXY();
}
