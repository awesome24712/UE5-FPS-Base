#include "BVector.h"

vec UVectorTools::DistanceBetweenSqr(const FVector& v1, const FVector& v2) {
    FVector dif = v1 - v2;
    return dif.SizeSquared();
}