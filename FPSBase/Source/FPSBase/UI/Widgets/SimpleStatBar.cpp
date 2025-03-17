#include "SimpleStatBar.h"
#include "../AwesomeGlass.h"

SimpleStatBar::SimpleStatBar() {

}

void SimpleStatBar::Draw(AUIConductor* c) {

	c->DrawRect(FCOLOR_TAN_DARK, realX, realY, realW * m_flPercentage, realH);

	c->DrawTexture(SharedAssets::statbarmeter, realX, realY, realW, realH, 0, 0, 1, 1);
}
