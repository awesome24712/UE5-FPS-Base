// Fill out your copyright notice in the Description page of Project Settings.


#include "UIConductor.h"
#include "UIHelpers.h"
#include "../BGPlayer.h"
#include "../BGController.h"

AUIConductor::AUIConductor() {

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

	FString v = GetOwningPawn() ? UIHelpers::VectorToString(GetOwningPawn()->GetVelocity()) : "MISSING PAWN";

	DrawText(v, FColor::White, 0, 0);
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

bool AUIConductor::WantsCursor() {
	return false;
}

ABGPlayer* AUIConductor::GetPlayer() {
	return  PlayerOwner->GetPawn<ABGPlayer>();
}
