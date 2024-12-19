// Fill out your copyright notice in the Description page of Project Settings.


#include "BGController.h"
#include "UI/UIConductor.h"
#include "BGPlayer.h"

ABGController::ABGController() {
	
}

void ABGController::BeginPlay() {
	Super::BeginPlay();
	
	//Setup UI conductor
	SetupUIConductor();
	//TODO open main menu

	FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
}

void ABGController::PlayerTick(float deltaTime) {
	Super::PlayerTick(deltaTime);

	auto pPlayer = GetPlayerCharacter();


	//mouse cursor control
	bool showNewMouseCursor = false; //default false, then check otherwise

	if ((GetUIC() && GetUIC()->WantsCursor())
		|| pPlayer && pPlayer->WantsCursor()) {
		showNewMouseCursor = true;
	}

	if (showNewMouseCursor != bShowMouseCursor) {
		bShowMouseCursor = showNewMouseCursor;

		//setup up mouse cursor
		if (showNewMouseCursor) {
			FInputModeGameAndUI inputMode;
			inputMode.SetHideCursorDuringCapture(false);
			inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
			SetInputMode(inputMode);

			auto& slate = FSlateApplication::Get();
			FVector2D cursor = slate.GetCursorPos();
			slate.SetCursorPos(cursor + FVector2D(0, 1));
			slate.OnMouseMove();
			slate.SetCursorPos(cursor);
			slate.OnMouseMove();
		}
		else {
			FInputModeGameOnly inputMode;
			SetInputMode(inputMode);
		}
	}
}

void ABGController::SetupInputComponent() {
	Super::SetupInputComponent();

#define bind(func) InputComponent->BindAction(#func, EInputEvent::IE_Released, this, &ABGController::func)
	bind(OnToggleMainMenu);
	bind(OnToggleTeamMenu);
	bind(OnToggleClassMenu);
	bind(OnToggleAdminMenu);
	bind(OnToggleCommMenu1);
	bind(OnToggleCommMenu2);
	bind(OnToggleCommMenu3);
	bind(OnToggleScoreboard);
	bind(OnEnterMapMaker);
#undef bind


}

void ABGController::SetupUIConductor() {
	ClientSetHUD(AUIConductor::StaticClass());
	m_uiConductor = Cast<AUIConductor>(MyHUD);// TObjectPtr<AUIConductor>(MyHUD);
}

bool ABGController::IsInMenu() const {
	return bShowMouseCursor;
}

bool ABGController::IsTyping() const {
	const auto& slate = FSlateApplication::Get();
	TSharedPtr<SWidget> widget = slate.GetUserFocusedWidget(0);

	return widget.IsValid() && widget.Get()->GetTypeAsString() == "SEditableText";
}

FVector2D ABGController::GetCursorPos() const {
	FVector2D result;
	ULocalPlayer* localPlayer = Cast<ULocalPlayer>(Player);

	if (localPlayer && localPlayer->ViewportClient) {
		localPlayer->ViewportClient->GetMousePosition(result);
	}

	return result;
}

ABGPlayer* ABGController::GetPlayerCharacter()
{
	return GetPawn<ABGPlayer>();
}

void ABGController::OnToggleMainMenu() {
	Msg(__FUNCTION__);
}

void ABGController::OnToggleTeamMenu() {
	Msg(__FUNCTION__);
}

void ABGController::OnToggleClassMenu() {
	Msg(__FUNCTION__);
}

void ABGController::OnToggleAdminMenu() {
	Msg(__FUNCTION__);
}

void ABGController::OnToggleCommMenu1() {
	Msg(__FUNCTION__);
}

void ABGController::OnToggleCommMenu2() {
	Msg(__FUNCTION__);
}

void ABGController::OnToggleCommMenu3() {
	Msg(__FUNCTION__);
}

void ABGController::OnToggleScoreboard() {
	Msg(__FUNCTION__);
}

void ABGController::OnEnterMapMaker() {
	Msg(__FUNCTION__);
}
