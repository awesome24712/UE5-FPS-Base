// Fill out your copyright notice in the Description page of Project Settings.


#include "UIConductor.h"
#include "UIHelpers.h"
#include "../BGPlayer.h"
#include "../BGController.h"
#include "Crosshair/Crosshair.h"
#include "AwesomeGlass.h"
#include "Widgets/AwesomeChat.h"
#include "../FPSBasePlayerState.h"

using namespace UIHelpers;

AUIConductor::AUIConductor() {
	m_flUiScale = 1.0f;
}


void AUIConductor::Setup() {
	if (!GEngine || m_bHasDoneUISetup) {
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

	m_bHasDoneUISetup = true;
}

void AUIConductor::ShowHUD() {
	Super::ShowHUD();
}

void AUIConductor::PostInitializeComponents() {
	Super::PostInitializeComponents();
	Setup();
	if (GEngine && GEngine->GameViewport) // make sure our screen is ready for the widget
	{
		SAssignNew(m_chatWidget, SAwesomeChatWidget).OwnerHUD(this); // add the widget and assign it to the var
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(m_chatWidget));
	}
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
	//but we can do slate stuff

	if (GetController()->WasInputKeyJustPressed(EKeys::Y) && m_chatWidget.IsValid()) {
		if (m_chatWidget->ChatInput.IsValid()) {
			FSlateApplication::Get().SetKeyboardFocus(m_chatWidget->ChatInput);
		}
	}
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

void AUIConductor::AddMessage(const int32 type, const FString& username, const FString& text, const bool replicate) {
	if (!GetController() || !m_chatWidget.IsValid())
		return;

	FChatMsg newMsg;
	newMsg.Init(type, FText::FromString(username), FText::FromString(text));
	if (newMsg.m_type > 0) {
		if (replicate) {
			auto ps = GetPlayerState();
			if (ps) {
				ps->UserChatRPC(newMsg);
			}
		}
		else {
			GetChatWidget()->AddMessage(newMsg);
		}
	}
}

bool AUIConductor::WantsCursor() const {
	return false;
}

ABGPlayer* AUIConductor::GetPlayer() const {
	return  PlayerOwner->GetPawn<ABGPlayer>();
}

ABGController* AUIConductor::GetController() const {
	return Cast<ABGController>(GetPlayer()->Controller);;
}

AFPSBasePlayerState* AUIConductor::GetPlayerState() const {
	return Cast<AFPSBasePlayerState>(GetPlayer()->Controller->PlayerState);;
}

FIntPoint AUIConductor::ScreenSize() const {
	return GEngine->GameViewport->Viewport->GetSizeXY();
}
