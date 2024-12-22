#pragma once
#include "CoreMinimal.h"
#include "../../Util.h"
#include "../UIConductor.h"
#include "../UIHelpers.h"

class AwesomeGlass {
public:
	AwesomeGlass();
//-----------------------------------------------------------------
// variables
//----------------------------------------------------------------- 
	float					X, Y, W, H; //if you ever modify these outside of the context of a screen-resolution-change, re-call PerformLayout() for them to apply
	FString					m_sTooltip; //if empty, no tooltip shows
	FColor					m_backgroundColor;
	FString					m_backgroundTexturePath; //if present, will be drawn, otherwise uses background color

	inline bool				IsMouseOver() const { return m_bMouseOver; }

private:
	float					realX, realY, realW, realH; //private, calculated based on alignment and scaling, ideally calculated only after screen size changes
	TObjectPtr<UTexture>	m_backgroundTexture; //if present, will be drawn, otherwise uses background color
	bool					m_bMouseOver;

//-----------------------------------------------------------------
// overridable interface, some with default implementations that
// that should be called from overrides
//-----------------------------------------------------------------
public:
	virtual const FString&	GetTooltip() const { return m_sTooltip; }
	virtual void			LoadAssets(); //default implementation loads background texture
	virtual void			Draw(AUIConductor* c); //default implementation draws background, called per-frame
	virtual void			PerformLayout(); //turns given coordinates into real coordinates, override for more custom alignments

	//mouse interactions

	virtual void			OnMouseBeginHover() {}
	virtual void			OnMouseEndHover() {}
	virtual void			OnMouseClick() {}



};


