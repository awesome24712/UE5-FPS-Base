#pragma once
#include "CoreMinimal.h"
#include "../../Util.h"
#include "../UIConductor.h"
#include "../UIHelpers.h"
#include "../../PODT/BColor.h"
#include "Engine/Font.h"

enum class EGlassAlignment {
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT,
	LEFT_CENTER,
	CENTER,
	RIGHT_CENTER,
	BOTTOM_LEFT,
	BOTTOM_CENTER,
	BOTTOM_RIGHT
};

#define AWESOME_GLASS_BINDINGS() \
	JT_BIND_FLOAT(X, "X", true); \
	JT_BIND_FLOAT(Y, "Y", true); \
	JT_BIND_FLOAT(W, "W", false); \
	JT_BIND_FLOAT(H, "H", false); \
	JT_BIND_COLOR(m_backgroundColor, "bgColor", false); \
	JT_BIND_STRING(m_backgroundTexturePath, "texture", false); \
	JT_BIND_STRING(m_sAlignment, "align", false); 

class AwesomeGlass : public IJsonBindable {
public:
	AwesomeGlass(bool bSkipBinding = false);
//-----------------------------------------------------------------
// variables
//----------------------------------------------------------------- 
	float					X, Y, W, H; //if you ever modify these outside of the context of a screen-resolution-change, re-call PerformLayout() for them to apply
	FString					m_sTooltip; //if empty, no tooltip shows
	FColor					m_backgroundColor;
	FString					m_backgroundTexturePath; //if present, will be drawn, otherwise uses background color

	inline bool				IsMouseOver() const { return m_bMouseOver; }
	inline bool				IsVisible() const { return m_bVisible; }

	float					realX, realY, realW, realH; //private, calculated based on alignment and scaling, ideally calculated only after screen size changes
protected:
	TObjectPtr<UTexture>	m_backgroundTexture; //if present, will be drawn, otherwise uses background color
	bool					m_bMouseOver;
	bool					m_bVisible;
	EGlassAlignment			m_eAlignment;
	FString					m_sAlignment; //translated into EAlignment

	TArray<AwesomeGlass*>	m_children;
	void					AddChild(AwesomeGlass* ag) { m_children.Push(ag); }

//-----------------------------------------------------------------
// overridable interface, some with default implementations that
// that should be called from overrides
//-----------------------------------------------------------------
public:
	virtual const FString&	GetTooltip() const { return m_sTooltip; }
	virtual void			LoadAssets(); //default implementation loads background texture
	virtual void			Draw(AUIConductor* c); //default implementation draws background, called per-frame
	virtual void			PerformLayout(AUIConductor* c); //turns given coordinates into real coordinates, override for more custom alignments
	virtual					~AwesomeGlass() {}

	//mouse interactions

	virtual void			OnMouseBeginHover() {}
	virtual void			OnMouseEndHover() {}
	virtual void			OnMouseClick() {}
	virtual void			OnMouseRelease() {}

	//other stuff

	void					SetIsVisible(bool bVisible) { m_bVisible = bVisible; }
	inline void				SetAlignment(EGlassAlignment a) { m_eAlignment = a; }
	void					SetSizeToScreen();
	inline void				SetSize(float _W, float _H) { W = _W; H = _H; }
	inline void				SetPos(float _X, float _Y) { X = _X; Y = _Y; }
};




