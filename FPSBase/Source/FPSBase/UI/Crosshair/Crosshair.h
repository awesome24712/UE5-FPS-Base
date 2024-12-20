#pragma once
#include "../../Util.h"

class ABGPlayer;
class AUIConductor;

namespace Crosshair {

	class CrosshairSettings : public IJsonBindable {

		CrosshairSettings();

		float m_flScale;
		int m_iTextureIndex;

		bool m_bMakeDynamic;
		bool m_bTopLine;
		bool m_bRightLine;
		bool m_bBottomLine;
		bool m_bLeftLine;
		bool m_bShowHealthAndStamina;

		uint8 m_iColorR;
		uint8 m_iColorG;
		uint8 m_iColorB;
		uint8 m_iColorA;

		uint8 m_iHealthAndStaminaAlpha;

	};

	void LoadCrosshairSettings();
	void SaveCrosshairSettings();

	CrosshairSettings* GetCrosshairSettings();

	void DrawCrosshair(AUIConductor* c, ABGPlayer* pPlayer);

}