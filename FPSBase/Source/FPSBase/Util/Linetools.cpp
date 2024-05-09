#include "linetools.h"
#include "Components/LineBatchComponent.h"
//#include "System/system.h"
#include "../FPSBaseCharacter.h"
#include "UnrealEngine.h"

#define sqr(a)      ((a) * (a))
#define SPLINE_STEP 5.f

#define CHN_WORLDSTATIC  (1 << ECC_WorldStatic)
#define CHN_WORLDDYNAMIC (1 << ECC_WorldDynamic)
#define CHN_PAWN         (1 << ECC_Pawn)
#define CHN_VISIBILITY   (1 << ECC_Visibility)
#define CHN_CAMERA       (1 << ECC_Camera)
#define CHN_PHYSICSBODY  (1 << ECC_PhysicsBody)
#define CHN_DESTRUCTIBLE (1 << ECC_Destructible)
#define CHN_ALL                                                                     \
    (CHN_WORLDSTATIC | CHN_WORLDDYNAMIC | CHN_PAWN | CHN_CAMERA | CHN_PHYSICSBODY | \
     CHN_DESTRUCTIBLE)

FCollisionObjectQueryParams g_coqpDefault =
FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllObjects);

inline FCollisionQueryParams GetDefaultTraceParams(AActor** ppIgnored) {
    FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;

    if (ppIgnored) {
        for (uint8 i = 0; ppIgnored[i]; i++) {
            params.AddIgnoredActor(ppIgnored[i]);
        }
    }

    return params;
}

inline bool LineToolsReady() { return GetLocalPlayer() && GetLocalPlayer()->GetWorld(); }

void UTIL_TraceLine(FHitResult& t, const FVector& start, const FVector& end,
    AActor** ppIgnoredActors) {
    if (!LineToolsReady()) return;

    t.Init();
    GetLocalPlayer()->GetWorld()->LineTraceSingleByObjectType(
        t, start, end, g_coqpDefault, GetDefaultTraceParams(ppIgnoredActors));
}

void UTIL_TraceSpline(FHitResult& t, const FVector& start, FVector splineDirection,
    FVector force, uint16 maxIterations, SLineDrawParams* rendered,
    AActor** ppIgnoredActors) {
    if (!LineToolsReady()) return;

    t.Init();
    uint16 iterations = 1;
    splineDirection = splineDirection.GetClampedToSize(SPLINE_STEP, SPLINE_STEP);
    FVector splineStep = splineDirection;
    FVector origForce = force;
    FVector next;
    FVector previous = start;

    auto pfTraceOp = [&]() {
        t.Reset();
        next = start + splineDirection + force;

        GetLocalPlayer()->GetWorld()->LineTraceSingleByObjectType(
            t, previous, next, g_coqpDefault,
            GetDefaultTraceParams(ppIgnoredActors));
        previous = next;

        iterations++;
        force += origForce * iterations;
        splineDirection += splineStep;
    };

    if (rendered) {
        while (iterations <= maxIterations && t.Time > 0.9999f) {
            pfTraceOp();
            GetLocalPlayer()->GetWorld()->LineBatcher->DrawLine(
                t.TraceStart, t.TraceEnd, rendered->m_Color, SDPG_World,
                rendered->m_Thickness, rendered->m_Duration);
        }
    }
    else {
        while (iterations <= maxIterations && t.Time > 0.9999f) {
            pfTraceOp();
        }
    }
}

void UTIL_DrawLine(FVector start, FVector end, SLineDrawParams* rendered) {
    if (!LineToolsReady() || !GetLocalPlayer()->GetWorld()->LineBatcher) return;
    GetLocalPlayer()->GetWorld()->LineBatcher->DrawLine(start, end, rendered->m_Color,
        SDPG_World, rendered->m_Thickness,
        rendered->m_Duration);
}

void UTIL_DrawSpline(FVector start, FVector end, FVector force, FColor c,
    float thickness, ftime life) {
    if (!LineToolsReady() || !GetLocalPlayer()->GetWorld()->LineBatcher) return;

    ULineBatchComponent* lb = GetLocalPlayer()->GetWorld()->LineBatcher;

    FVector displacement = end - start;
    vec     len = displacement.Size();
    vec     splineStep = 0.0f;
    FVector vectorStep = start;
    FVector previousStep = vectorStep;
    while (splineStep < len) {
        float forceScale = 1.0f - 4 * sqr((splineStep / len) - 0.5f);
        vectorStep = start + displacement * splineStep / len;
        vectorStep += force * forceScale;
        lb->DrawLine(previousStep, vectorStep, c, SDPG_World, thickness, life);
        previousStep = vectorStep;
        splineStep += SPLINE_STEP;
    }
}

void UTIL_DrawCircle(FVector loc, vec radius, SLineDrawParams* rendered) {
    if (!LineToolsReady() || !GetLocalPlayer()->GetWorld()->LineBatcher) return;

    ULineBatchComponent* lb = GetLocalPlayer()->GetWorld()->LineBatcher;

    const uint8 NUM_ARCS = 64;
    FRotator    rotationIncrement = FRotator(0, 360.f / NUM_ARCS, 0);
    FVector     previous = FVector(radius, 0, 0);
    FVector     next = rotationIncrement.RotateVector(previous);

    for (uint8 i = 0; i < NUM_ARCS + 1; i++) {
        lb->DrawLine(loc + previous, loc + next, rendered->m_Color, SDPG_World,
            rendered->m_Thickness, rendered->m_Duration);
        previous = next;
        next = rotationIncrement.RotateVector(next);
    }
}

vec UTIL_DistanceToLine(FVector vDirection, FVector vOrigin, FVector point) {
    FVector toPoint = point - vOrigin;
    FVector projection = toPoint.ProjectOnTo(vDirection);
    return (toPoint - projection).Size();
}