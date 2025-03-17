#pragma once
#include "../AwesomeGlass/BaseGlass.h"

class RotatingCompass : public AwesomeGlass {
public:
	RotatingCompass();

	virtual void			Draw(AUIConductor* c); //default implementation draws background, called per-frame
	virtual void			PerformLayout(AUIConductor* c); //turns given coordinates into real coordinates, override for more custom alignments
};