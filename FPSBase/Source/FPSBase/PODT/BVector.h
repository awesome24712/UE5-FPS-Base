#include "CoreMinimal.h"
#include "../Util.h"
#include "BVector.generated.h"

USTRUCT()
struct BVector : public FVector, public IJsonBindable {
    BVector() {
        JT_START_BINDING("bvector", BVector);
        JT_BIND_FLOAT(X, "x", true);
        JT_BIND_FLOAT(Y, "y", true);
        JT_BIND_FLOAT(Z, "z", true);
        JT_FINISH_BINDING();
    }
}

UCLASS()
class VectorTools : public UObject {
    vec DistanceBetweenSqr(const FVector& v1, const FVector& v2);
    vec DistanceBetweenSqr(const AActor* a1, const AActor* a2) {
        return DistanceBetweenSqr(a1->GetAbsoluteLocation(), a2->GetAbsoluteLocation());
    }
    vec DistanceBetweenSqr(const AActor* a1, const FVector& v2) {
        return DistanceBetweenSqr(a1->GetAbsoluteLocation(), v2);
    }
    vec DistanceBetweenSqr(const Fector& v1, const AActor* a2) {
        return DistanceBetweenSqr(v1, a2->GetAbsoluteLocation());
    }
}