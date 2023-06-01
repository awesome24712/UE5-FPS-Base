#pragma once
#include "CoreMinimal.h"
#include "../Util.h"
#include "BVector.generated.h"

USTRUCT()
struct FBVector : public FVector, public IJsonBindable {
    GENERATED_BODY()
    FBVector() {
        JT_START_BINDING("bvector", FBVector);
        JT_BIND_FLOAT(X, "x", true);
        JT_BIND_FLOAT(Y, "y", true);
        JT_BIND_FLOAT(Z, "z", true);
        JT_FINISH_BINDING();
    }
};

UCLASS()
class UVectorTools : public UObject {
    GENERATED_BODY()
public:
    static vec DistanceBetweenSqr(const FVector& v1, const FVector& v2);
    static vec DistanceBetweenSqr(const AActor* a1, const AActor* a2) {
        return DistanceBetweenSqr(a1->GetActorLocation(), a2->GetActorLocation());
    }
    static vec DistanceBetweenSqr(const AActor* a1, const FVector& v2) {
        return DistanceBetweenSqr(a1->GetActorLocation(), v2);
    }
    static vec DistanceBetweenSqr(const FVector& v1, const AActor* a2) {
        return DistanceBetweenSqr(v1, a2->GetActorLocation());
    }
};