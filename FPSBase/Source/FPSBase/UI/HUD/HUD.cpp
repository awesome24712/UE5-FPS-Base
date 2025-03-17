#include "HUD.h"
#include "CoreMinimal.h"
#include "../AwesomeGlass.h"
#include "../../CommonHeaders.h"

AwesomeHUD::AwesomeHUD() {
	AddChild(m_pCompass = new RotatingCompass());
	AddChild(m_pHealthLabel = new TextGlass());
	AddChild(m_pAmmoCountLabel = new TextGlass());
	AddChild(m_pHealthBar = new AwesomeGlass());
}

void AwesomeHUD::LoadAssets() {
	const JsonTree* settings = NTreeManager::FindFile("hudSettings.json");
	m_pCompass->LoadBindingsFromJson(settings->GetChild("compass"));
	m_pHealthLabel->LoadBindingsFromJson(settings->GetChild("healthlabel"));
	m_pHealthBar->LoadBindingsFromJson(settings->GetChild("healthbar"));
	m_pAmmoCountLabel->LoadBindingsFromJson(settings->GetChild("ammocount"));

	AwesomeGlass::LoadAssets();
}

void AwesomeHUD::Draw(AUIConductor* c) {
	if (!IsVisible()) {
		return;
	}

	auto pPlayer = c->GetPlayer();

	//update health bar label text
	m_pHealthLabel->SetText(FString::Printf(TEXT("%i"), pPlayer->GetHealth()));

	//TODO update ammo count
	m_pAmmoCountLabel->SetText("1");

	//size the m_pHealthBar to match player health
	m_pHealthBar->realW = m_flHealthbarMaxPixelWidth * pPlayer->GetHealth() / pPlayer->GetMaxHealth();

	AwesomeGlass::Draw(c);

	//draw texture over healthbar
	auto hb = m_pHealthBar;
	c->DrawTexture(SharedAssets::statbarmeter, hb->realX, hb->realY, m_flHealthbarMaxPixelWidth, hb->realH, 0, 0, 1, 1);


}

void AwesomeHUD::PerformLayout(AUIConductor* c) {
	SetSizeToScreen();
	AwesomeGlass::PerformLayout(c);
	m_flHealthbarMaxPixelWidth = m_pHealthBar->realW;

	/*m_pCompass->SetAlignment(EGlassAlignment::BOTTOM_RIGHT);
	m_pCompass->SetSize(100, 100);
	m_pCompass->SetPos(120, 120);

	m_pCompass->PerformLayout(c);*/
	
}
