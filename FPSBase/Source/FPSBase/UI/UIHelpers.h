#pragma once
#include "CoreMinimal.h"

namespace UIHelpers {
	FString VectorToString(const FVector& v);

	//Returns X Y position for box to be centered
	FIntPoint CenterBoxOnPoint(FIntPoint boxSize, FIntPoint containerSize, FIntPoint point);
}