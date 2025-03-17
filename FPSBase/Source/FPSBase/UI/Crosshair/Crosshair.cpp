#include "Crosshair.h"
#include "../UIConductor.h"
#include "../UIHelpers.h"
#include "../../CommonHeaders.h"

using namespace UIHelpers;

namespace Crosshair {

	/*ConVar cr_crosshair_t("cl_crosshair_t", CVAR_PERSISTENT, true, "Whether or not to show crosshair texture");
	ConVar cr_crosshair_dot("cl_crosshair_dot", CVAR_PERSISTENT, true, "Whether or not to show crosshair dot");
	ConVar cr_crosshair_lines("cl_crosshair_lines", CVAR_PERSISTENT, 0b1111, "Whether or not to show crosshair lines, bitfield");
	ConVar cr_crosshair_circle("cl_crosshair_circle", CVAR_PERSISTENT, true, "Whether or not to show crosshair circle");

	ConVar cr_crosshair_color("cl_crosshair_color", CVAR_PERSISTENT, "(255,255,255,255)", "Crosshair tint colour");*/

	CrosshairSettings::CrosshairSettings() {
		JT_START_BINDING("crosshairSettings", CrosshairSettings);
		JT_BIND_FLOAT(m_flScale, "scale", true);
		JT_BIND_INT(m_iTextureIndex, "textureIndex", true);
		JT_BIND_BOOLEAN(m_bDot, "dot", true);
		JT_BIND_BOOLEAN(m_bMakeDynamic, "makeDynamic", true);
		JT_BIND_BOOLEAN(m_bTopLine, "topLine", true);
		JT_BIND_BOOLEAN(m_bRightLine, "rightLine", true);
		JT_BIND_BOOLEAN(m_bBottomLine, "bottomLine", true);
		JT_BIND_BOOLEAN(m_bLeftLine, "leftLine", true);
		JT_BIND_BOOLEAN(m_bCircle, "circle", true);
		JT_BIND_BOOLEAN(m_bShowHealthAndStamina, "showHealthAndStamina", true);
		JT_BIND_COLOR(m_color, "crosshairColor", true);
		JT_BIND_BYTE(m_iHealthAndStaminaAlpha, "healthAndStaminaAlpha", true);
		JT_FINISH_BINDING();
	}

	CrosshairSettings g_crosshairSettings;
	static CrosshairSettings& gs = g_crosshairSettings;

	TObjectPtr<UTexture> g_crosshair1;

	void LoadCrosshairSettings() {
		g_crosshair1 = LoadObject<UTexture>(nullptr, ANSI_TO_TCHAR("/Game/Materials/UI/Crosshair/T_crosshair0"));

		NTreeManager::LoadBindablesFromFile(&gs, "crosshairSettings.json");
		
	}

	void SaveCrosshairSettings() {

	}

	CrosshairSettings* GetCrosshairSettings() {
		return nullptr;
	}


	static float previousR = 50.f;
	void DrawCrosshair(AUIConductor* c, ABGPlayer* pPlayer) {
		float idealR = 50.f;

		if (pPlayer->GetVelocity().SquaredLength() > 1) {
			idealR *= 2;
		}

		float r = previousR * 0.8f + idealR * 0.2f;
		previousR = r;
		float s = gs.m_flScale;
		float cx = c->ScreenWidth() / 2;
		float cy = c->ScreenHeight() / 2;

		//----------------------------------------------------------------------
		// Dot
		//----------------------------------------------------------------------
		if (gs.m_bDot) {
			float ds = 1.0f * s;
			//float ds = 1.0f;
			float dsd = (ds - 1.0f) / 2;
			c->DrawRect(gs.m_color, cx - dsd, cy - dsd, ds, ds);
		}

		//----------------------------------------------------------------------
		// Lines
		//----------------------------------------------------------------------
		int lineLength = 5 * s;
		int lineSpacing = 5 * s;
		float ls = 1.0f * s;
		float lsd = (ls - 1.0f) / 2;
		if (gs.m_bTopLine) {
			c->DrawRect(gs.m_color, cx - lsd, cy - lineSpacing - lineLength, ls, lineLength);
		}
		if (gs.m_bRightLine) {
			c->DrawRect(gs.m_color, cx - lineSpacing - lineLength, cy - lsd, lineLength, ls);
		}
		if (gs.m_bBottomLine) {
			c->DrawRect(gs.m_color, cx - lsd, cy + lineSpacing, ls, lineLength);
		}
		if (gs.m_bLeftLine) {
			c->DrawRect(gs.m_color, cx + lineSpacing, cy - lsd, lineLength, ls);
		}


		//----------------------------------------------------------------------
		// Circle
		//----------------------------------------------------------------------
		if (gs.m_bCircle) {
	
			int step = 10;
			float r2 = r * 0.95f;
			FColor thirdColor = gs.m_color;
			thirdColor.A /= 3;
			
			for (int dx = -1; dx <= 1; dx++)
				for (int dy = -1; dy <= 1; dy++)
				{
					if (dx == 0 && dy == 0)
						continue;

					int cx2 = cx + dx,
						cy2 = cy + dy;

					int lastx = (int)(cx2 + r2 * cosf((float)-step * PI / 180.f)),
						lasty = (int)(cy2 + r2 * sinf((float)-step * PI / 180.f));

					for (int i = 0, j = 0; i < 360; i += step, j++)
					{
						float	a = (float)i * PI / 180.f;

						int x = (int)(cx2 + r2 * cosf(a)),
							y = (int)(cy2 + r2 * sinf(a));

						c->Draw2DLine(lastx, lasty, x, y, thirdColor);
						
						lastx = x;
						lasty = y;
					}
				}

			int lastx = (int)(cx + r2 * cosf((float)-step * PI / 180.f)),
				lasty = (int)(cy + r2 * sinf((float)-step * PI / 180.f));


			for (int i = 0, j = 0; i < 360; i += step, j++)
			{
				float	a = (float)i * PI / 180.f;

				int x = (int)(cx + r2 * cosf(a)),
					y = (int)(cy + r2 * sinf(a));

				c->Draw2DLine(lastx, lasty, x, y, gs.m_color);

				lastx = x;
				lasty = y;
			}
		}
		


		//----------------------------------------------------------------------
		// Texture
		//----------------------------------------------------------------------
		if (gs.m_iTextureIndex) {
			FIntPoint size(r * 2, r * 2);
			FIntPoint loc = CenterBoxOnPoint(size, c->ScreenSize() / 2);
			c->DrawTexture(g_crosshair1, loc.X, loc.Y, size.X, size.Y, 0, 0, 1, 1);
		}
	}
	
}


