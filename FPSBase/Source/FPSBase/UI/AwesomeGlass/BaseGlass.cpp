#include "BaseGlass.h"

AwesomeGlass::AwesomeGlass(bool bSkipBinding) :
	X(0), Y(0), W(0), H(0),
	m_sTooltip(TEXT("")),
	m_backgroundColor(0, 0, 0, 0),
	m_backgroundTexturePath(TEXT("")),
	realX(0), realY(0), realW(0), realH(0),
	m_bMouseOver(false),
	m_bVisible(true),
	m_eAlignment(EGlassAlignment::TOP_LEFT),
	m_sAlignment(TEXT(""))
{

	if (!bSkipBinding) {
		JT_START_BINDING("awesomeGlass", AwesomeGlass);
		AWESOME_GLASS_BINDINGS();
		JT_FINISH_BINDING();
	}
}

void AwesomeGlass::LoadAssets() {
	m_backgroundTexture = LoadObject<UTexture>(nullptr, *m_backgroundTexturePath);
}

void AwesomeGlass::Draw(AUIConductor* c) {
	if (m_backgroundTexture.Get()) {
		c->DrawTexture(m_backgroundTexture, realX, realY, realW, realH, 0, 0, 1, 1);
	}
	else if (m_backgroundColor.Bits) {
		c->DrawRect(m_backgroundColor, realX, realY, realW, realH);
	}
}

void AwesomeGlass::PerformLayout(AUIConductor* c) {
	//first copy
	realX = X; realY = Y; realW = W; realH = H;

	//scale
	float scale = c->GetScale();
	realX *= scale; realY *= scale; realW *= scale; realH *= scale;

	//do alignments
	//first, make right-aligned items use coordinates from the right
	EGlassAlignment& e = m_eAlignment;
	if (e == EGlassAlignment::TOP_RIGHT || e == EGlassAlignment::RIGHT_CENTER || e == EGlassAlignment::BOTTOM_RIGHT) {
		realX = c->ScreenWidth() - realX;
	}
	//make bottom-align items use coordinates from the top
	if (e == EGlassAlignment::BOTTOM_LEFT || e == EGlassAlignment::BOTTOM_CENTER || e == EGlassAlignment::BOTTOM_RIGHT) {
		realY = c->ScreenHeight() - realY;
	}

	//center alignments
	if (e == EGlassAlignment::TOP_CENTER || e == EGlassAlignment::CENTER || e == EGlassAlignment::BOTTOM_CENTER) {
		realX = realX + (c->ScreenWidth() / 2) - (realW / 2);
	}
	if (e == EGlassAlignment::LEFT_CENTER || e == EGlassAlignment::CENTER || e == EGlassAlignment::RIGHT_CENTER) {
		realX = realY + (c->ScreenHeight() / 2) - (realH / 2);
	}
}
