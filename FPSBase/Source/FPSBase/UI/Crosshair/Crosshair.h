#pragma once
#include "../../Util.h"

class ABGPlayer;
class AUIConductor;

namespace Crosshair {

	class CrosshairSettings : public IJsonBindable {

	public:
		CrosshairSettings();

		float m_flScale = 1.0f;
		int m_iTextureIndex = 1;

		bool m_bDot = true;
		bool m_bMakeDynamic = true;
		bool m_bTopLine = true;
		bool m_bRightLine = true;
		bool m_bBottomLine = true;
		bool m_bLeftLine = true;
		bool m_bCircle = true;
		bool m_bShowHealthAndStamina = true;

		FColor m_color = FCOLOR_WHITE;

		uint8 m_iHealthAndStaminaAlpha = 255;

	};

	void LoadCrosshairSettings();
	void SaveCrosshairSettings();

	CrosshairSettings* GetCrosshairSettings();

	void DrawCrosshair(AUIConductor* c, ABGPlayer* pPlayer);

}