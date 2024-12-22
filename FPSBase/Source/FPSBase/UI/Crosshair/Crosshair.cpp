#include "Crosshair.h"
#include "../UIConductor.h"
#include "../UIHelpers.h"

using namespace UIHelpers;

namespace Crosshair {

	CrosshairSettings::CrosshairSettings() {

	}

	TObjectPtr<UTexture> g_crosshair0;

	void LoadCrosshairSettings() {
		/*ConstructorHelpers::FObjectFinder<UTexture> crosshair0(TEXT("Materials/UI/Crosshair/T_crosshair0"));
		g_crosshair0 = crosshair0.Object;*/
		g_crosshair0 = LoadObject<UTexture>(nullptr, ANSI_TO_TCHAR("/Game/Materials/UI/Crosshair/T_crosshair0"));


		/*if (!g_crosshair0) {
			NLogger::Fatal("Couldn't load crosshair texture");
		}
		else {
			NLogger::Fatal("Loaded texture!");
		}*/
	}

	void SaveCrosshairSettings() {

	}

	CrosshairSettings* GetCrosshairSettings() {
		return nullptr;
	}

	void DrawCrosshair(AUIConductor* c, ABGPlayer* pPlayer) {
		
		FIntPoint size(200, 200);
		FIntPoint loc = CenterBoxOnPoint(size, c->ScreenSize(), c->ScreenSize() / 2);

		//Msg("%i %i", loc.X, loc.Y);

		//c->DrawTextureSimple(g_crosshair0, 0, 0);
		c->DrawText("DWAOIDHAWUDHWA", FColor::Red, loc.X, loc.Y, nullptr, 5.f);
		c->DrawTexture(g_crosshair0, loc.X, loc.Y, size.X, size.Y, 0, 0, 1, 1);
		//c->DrawRect(FColor(255, 0, 0, 128), loc.X, loc.Y, size.X, size.Y);
	}
	
}


