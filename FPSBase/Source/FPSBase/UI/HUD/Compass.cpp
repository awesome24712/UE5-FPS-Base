#include "Compass.h"
#include "../../CommonHeaders.h"
#include "../AwesomeGlass.h"

RotatingCompass::RotatingCompass() {

}

void RotatingCompass::Draw(AUIConductor* c) {
	auto pPlayer = c->GetController();
	
	FRotator rotation = pPlayer->PlayerCameraManager->GetCameraRotation();
	double yaw = rotation.Yaw; // -180 to +180

	c->DrawTexture(SharedAssets::compass, realX, realY, realW, realH,
		0, 0, 1, 1, FLinearColor::White, BLEND_Translucent, 1.0f, false, 
		yaw, FVector2D(0.5, 0.5));

	//Draw E, S, W textures
	//calculate rotation-based coordinates in radians
	float yawE = (yaw + 0) * PI / 180;
	float yawS = (yaw + 90) * PI / 180;
	float yawW = (yaw + 180) * PI / 180;

	const float iconSize = 20;
	const float rd = 2.2f; //radius divisor, increase to decrease radius

	float EX = realX + (realW / 2) + cosf(yawE) * (realW / rd) - (iconSize / 2);
	float EY = realY + (realH / 2) + sinf(yawE) * (realH / rd) - (iconSize / 2);
	float SX = realX + (realW / 2) + cosf(yawS) * (realW / rd) - (iconSize / 2);
	float SY = realY + (realH / 2) + sinf(yawS) * (realH / rd) - (iconSize / 2);
	float WX = realX + (realW / 2) + cosf(yawW) * (realW / rd) - (iconSize / 2);
	float WY = realY + (realH / 2) + sinf(yawW) * (realH / rd) - (iconSize / 2);

	c->DrawTexture(SharedAssets::compassE, EX, EY, 20, 20, 0, 0, 1, 1);
	c->DrawTexture(SharedAssets::compassS, SX, SY, 20, 20, 0, 0, 1, 1);
	c->DrawTexture(SharedAssets::compassW, WX, WY, 20, 20, 0, 0, 1, 1);




	/*c->DrawText(FString::Printf(TEXT("(%.2f, %.2f, %.2f, %.2f)"), yawE, EX, EY, 0.f), 
		FColor::White, c->ScreenWidth() - 200, 70);*/


	

}

void RotatingCompass::PerformLayout(AUIConductor* c) {
	AwesomeGlass::PerformLayout(c);
}
