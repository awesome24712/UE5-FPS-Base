#include "UIHelpers.h"

FString UIHelpers::VectorToString(const FVector& v)
{
	return FString::Printf(TEXT("(%.2f, %.2f, %.2f)"), v.X, v.Y, v.Z);
}

FIntPoint UIHelpers::CenterBoxOnPoint(FIntPoint boxSize, FIntPoint containerSize, FIntPoint point)
{
	int32 X = (containerSize.X - boxSize.X) / 2;
	int32 Y = (containerSize.Y - boxSize.Y) / 2;
	return FIntPoint(X, Y);
}
