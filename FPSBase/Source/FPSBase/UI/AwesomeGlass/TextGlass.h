#pragma once
#include "BaseGlass.h"

class TextGlass : public AwesomeGlass {
public:
	TextGlass();
	//-----------------------------------------------------------------
	// variables
	//----------------------------------------------------------------- 


private:


	//-----------------------------------------------------------------
	// overridable interface, some with default implementations that
	// that should be called from overrides
	//-----------------------------------------------------------------
public:
	virtual void			LoadAssets()			override; //default implementation loads background texture
	virtual void			Draw(AUIConductor* c)	override; //default implementation draws background, called per-frame
	virtual void			PerformLayout(AUIConductor* c)			override; //turns given coordinates into real coordinates, override for more custom alignments

};