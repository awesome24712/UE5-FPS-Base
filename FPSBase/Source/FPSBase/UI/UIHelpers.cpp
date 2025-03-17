#include "UIHelpers.h"
#include "Engine/Font.h"

FString UIHelpers::VectorToString(const FVector& v)
{
	return FString::Printf(TEXT("(%.2f, %.2f, %.2f)"), v.X, v.Y, v.Z);
}

FString UIHelpers::RotatorToString(const FRotator& r)
{
	return FString::Printf(TEXT("(%.2f, %.2f, %.2f)"), r.Pitch, r.Yaw, r.Roll);
}

FIntPoint UIHelpers::CenterBoxOnPoint(FIntPoint boxSize, FIntPoint point)
{
	int32 X = point.X - (boxSize.X / 2);
	int32 Y = point.Y - (boxSize.Y / 2);
	return FIntPoint(X, Y);
}

namespace SharedAssets {
	TObjectPtr<UTexture> ammoball;
	TObjectPtr<UTexture> compass;
	TObjectPtr<UTexture> flag_cap_blank;
	TObjectPtr<UTexture> heart;
	TObjectPtr<UTexture> statbarmeter;

	TObjectPtr<UTexture> compassE;
	TObjectPtr<UTexture> compassS;
	TObjectPtr<UTexture> compassW;

	TObjectPtr<UFont> corsiva = NULL;

	void LoadSharedAssets() {
		ammoball		= LoadObject<UTexture>(nullptr, ANSI_TO_TCHAR("/Game/Materials/UI/HUD/ammoball"));
		compass			= LoadObject<UTexture>(nullptr, ANSI_TO_TCHAR("/Game/Materials/UI/HUD/compass"));
		flag_cap_blank	= LoadObject<UTexture>(nullptr, ANSI_TO_TCHAR("/Game/Materials/UI/HUD/flag_cap_blank"));
		heart			= LoadObject<UTexture>(nullptr, ANSI_TO_TCHAR("/Game/Materials/UI/HUD/heart"));
		statbarmeter	= LoadObject<UTexture>(nullptr, ANSI_TO_TCHAR("/Game/Materials/UI/ClassMenu/statbarmeter"));

		compassE = LoadObject<UTexture>(nullptr, ANSI_TO_TCHAR("/Game/Materials/UI/HUD/compassE"));
		compassS = LoadObject<UTexture>(nullptr, ANSI_TO_TCHAR("/Game/Materials/UI/HUD/compassS"));
		compassW = LoadObject<UTexture>(nullptr, ANSI_TO_TCHAR("/Game/Materials/UI/HUD/compassW"));

		corsiva = LoadObject<UFont>(nullptr, ANSI_TO_TCHAR("/Game/Fonts/MTCORSVA_Font"));
	}
}