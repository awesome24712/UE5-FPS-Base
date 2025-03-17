#pragma once
#include "../AwesomeGlass/BaseGlass.h"

class SimpleStatBar : public AwesomeGlass {
public:
	SimpleStatBar();

	float m_flPercentage = 1.0f; //what percent of the stat bar should be filled?

	virtual void			Draw(AUIConductor* c); //default implementation draws children and background, called per-frame
};