#pragma once
#include "CoreMinimal.h"



namespace UIHelpers {
	FString VectorToString(const FVector& v);

	FString RotatorToString(const FRotator& v);

	//Returns X Y position for box to be centered
	FIntPoint CenterBoxOnPoint(FIntPoint boxSize, FIntPoint point);
}

namespace SharedAssets {
	extern TObjectPtr<UTexture> ammoball;
	extern TObjectPtr<UTexture> compass;
	extern TObjectPtr<UTexture> flag_cap_blank;
	extern TObjectPtr<UTexture> heart;
	extern TObjectPtr<UTexture> statbarmeter;

	extern TObjectPtr<UTexture> compassE;
	extern TObjectPtr<UTexture> compassS;
	extern TObjectPtr<UTexture> compassW;

	extern TObjectPtr<UFont> corsiva;

	void LoadSharedAssets();
}