#include "UIHelpers.h"

FString UIHelpers::VectorToString(const FVector& v)
{
	return FString::Printf(TEXT("(%.2f, %.2f, %.2f)"), v.X, v.Y, v.Z);
}
