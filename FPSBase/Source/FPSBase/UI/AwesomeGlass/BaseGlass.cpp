#include "BaseGlass.h"

AwesomeGlass::AwesomeGlass() :
	X(0), Y(0), W(0), H(0),
	m_sTooltip(TEXT("")),
	m_backgroundColor(0, 0, 0, 0),
	m_backgroundTexturePath(TEXT("")),
	realX(0), realY(0), realW(0), realH(0),
	m_bMouseOver(false)
{

}

void AwesomeGlass::LoadAssets() {

}

void AwesomeGlass::Draw(AUIConductor* c) {

}

void AwesomeGlass::PerformLayout() {

}
