#pragma once
#include "BaseGlass.h"


class TextGlass : public AwesomeGlass {
public:
	TextGlass();
	TextGlass(const FString& text, UFont* font = NULL, FColor c = FCOLOR_WHITE, uint8 size = 9);


	//-----------------------------------------------------------------
	// variables
	//----------------------------------------------------------------- 
private:
	FString m_text; //text to display
	UFont* m_pFont;
	uint8 m_iFontSize;
	FColor m_color;

	//-----------------------------------------------------------------
	// overridable interface, some with default implementations that
	// that should be called from overrides
	//-----------------------------------------------------------------
public:
	virtual void			LoadAssets()			override; //default implementation loads background texture
	virtual void			Draw(AUIConductor* c)	override; //default implementation draws background, called per-frame
	virtual void			PerformLayout(AUIConductor* c)			override; //turns given coordinates into real coordinates, override for more custom alignments

	void					SetText(const FString& text);
	const FString&			GetText() const { return m_text; }
	void					SetFontSize(uint8 size) { m_iFontSize = size; }
	void					SetFont(UFont* font) { m_pFont = font; }
	void					SetColor(FColor c) { m_color = c; }
};