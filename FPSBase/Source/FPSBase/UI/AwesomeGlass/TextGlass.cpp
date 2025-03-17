#include "TextGlass.h"

#define TEXT_BINDINGS() \
	JT_START_BINDING("textGlass", TextGlass); \
	AWESOME_GLASS_BINDINGS(); \
	JT_BIND_STRING(m_text, "text", false); \
	JT_BIND_BYTE(m_iFontSize, "fontSize", false); \
	JT_BIND_COLOR(m_color, "color", false); \
	JT_FINISH_BINDING(); 

TextGlass::TextGlass() : AwesomeGlass(true) {
	m_pFont = SharedAssets::corsiva;
	m_color = FCOLOR_WHITE;
	m_iFontSize = 9;

	TEXT_BINDINGS();
}

TextGlass::TextGlass(const FString& text, UFont* font, FColor c, uint8 size) : 
	AwesomeGlass (true),
	m_pFont(font),
	m_iFontSize(size),
	m_color(c)
{
	SetText(text);
	if (!font) {
		m_pFont = SharedAssets::corsiva;
	}

	TEXT_BINDINGS();
}

void TextGlass::LoadAssets() {

}

void TextGlass::Draw(AUIConductor* c) {
	//if (!m_pFont) return;
	if (m_pFont) {
		m_pFont->LegacyFontSize = m_iFontSize;
	}
	

	c->DrawText(m_text, m_color, realX, realY, m_pFont);
}

void TextGlass::PerformLayout(AUIConductor* c) {
	AwesomeGlass::PerformLayout(c);
}

void TextGlass::SetText(const FString& text) {
	if (((char)text[0]) == '#') {
		m_text = LangSystem::Localize(text);
	}
	else {
		m_text = text;
	}
}
