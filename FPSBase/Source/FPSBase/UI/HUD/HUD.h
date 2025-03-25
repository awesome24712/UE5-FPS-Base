#pragma once
#include "../AwesomeGlass/BaseGlass.h"

class RotatingCompass;
class TextGlass;
class BaseGlass;

class AwesomeHUD : public AwesomeGlass {

public:

	AwesomeHUD();

	//overridesA
	virtual void			LoadAssets(); //default implementation loads background texture
	virtual void			Draw(AUIConductor* c); //default implementation draws background, called per-frame
	virtual void			PerformLayout(AUIConductor* c); //turns given coordinates into real coordinates, override for more custom alignments

	//HUD elements
	RotatingCompass*			m_pCompass;
	TextGlass* m_pHealthLabel;
	TextGlass* m_pAmmoCountLabel;
	AwesomeGlass* m_pHealthBar;
	float	m_flHealthBarMaxPixelWidth;
	AwesomeGlass* m_pStaminaBar;
	float	m_flStaminaBarMaxPixelWidth;
	
};