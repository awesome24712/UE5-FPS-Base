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
	if (!m_backgroundTexturePath.IsEmpty()) {
		m_backgroundTexture = LoadObject<UTexture>(nullptr, *m_backgroundTexturePath);
	}

	for (auto child : m_children) {
		child->LoadAssets();
	}
}

void AwesomeGlass::Draw(AUIConductor* c) {
	if (m_backgroundTexture.Get()) {
		c->DrawTexture(m_backgroundTexture, realX, realY, realW, realH, 0, 0, 1, 1);
	}
	else if (m_backgroundColor.Bits) {
		c->DrawRect(m_backgroundColor, realX, realY, realW, realH);
	}
	for (auto child : m_children) {
		child->Draw(c);
	}
}

void AwesomeGlass::PerformLayout(AUIConductor* c) {
	//first copy
	realX = X; realY = Y; realW = W; realH = H;

	//scale
	float scale = c->GetScale();
	realX *= scale; realY *= scale; realW *= scale; realH *= scale;

	//if we have a string-based alignment and (probably) haven't aligned yet, load that
	if (!m_sAlignment.IsEmpty() && m_eAlignment == EGlassAlignment::TOP_LEFT) {
		FString& s = m_sAlignment;
		if (s[0] == 't' && s[1] == 'l') {
			m_eAlignment = EGlassAlignment::TOP_LEFT;
		}
		else if (s[0] == 't' && s[1] == 'c') {
			m_eAlignment = EGlassAlignment::TOP_CENTER;
		}
		else if (s[0] == 't' && s[1] == 'r') {
			m_eAlignment = EGlassAlignment::TOP_RIGHT;
		}
		else if (s[0] == 'c' && s[1] == 'l') {
			m_eAlignment = EGlassAlignment::LEFT_CENTER;
		}
		else if (s[0] == 'c' && s[1] == 'c') {
			m_eAlignment = EGlassAlignment::CENTER;
		}
		else if (s[0] == 'c' && s[1] == 'r') {
			m_eAlignment = EGlassAlignment::RIGHT_CENTER;
		}
		else if (s[0] == 'b' && s[1] == 'l') {
			m_eAlignment = EGlassAlignment::BOTTOM_LEFT;
		}
		else if (s[0] == 'b' && s[1] == 'c') {
			m_eAlignment = EGlassAlignment::BOTTOM_CENTER;
		}
		else if (s[0] == 'b' && s[1] == 'r') {
			m_eAlignment = EGlassAlignment::BOTTOM_RIGHT;
		}
	}

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
		realY = realY + (c->ScreenHeight() / 2) - (realH / 2);
	}

	for (auto child : m_children) {
		child->PerformLayout(c);
	}
}

void AwesomeGlass::SetSizeToScreen() {
	FIntPoint screenSize = GEngine->GameViewport->Viewport->GetSizeXY();
	X = Y = 0;
	H = screenSize.Y;
	W = screenSize.X;
}
