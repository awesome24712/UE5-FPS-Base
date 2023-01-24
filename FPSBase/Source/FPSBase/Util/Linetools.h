
#pragma once

#include "Predefs.h"
#include "UnrealEngine.h"

 // contains parameters used for drawing lines and splines in the world
struct SLineDrawParams {
    FColor Color;
    float  Thickness;
    ftime  Duration;
};

/*
 * Given a start position, direction, null-terminated list of ignored actors,
 * and max distance, performs a ray cast and stores the result in the given
 * FHitResult
 */
void UTIL_TraceLine(FHitResult& t, const FVector& start, FVector direction,
    AActor** ppIgnoredActors = NULL,
    float    maxDistance = (float)INT32_MAX);

/* Given a start position, direction, force, and maximum number of iterations
 * performs a spline cast and stores the result in the given FHitResult.
 * The magnitude of the direction does not effect the trace.
 * The direction and magnitude of force determines the curvature of the spline;
 * i.e. the force vector causes an acceleration on the spline curve.
 */
void UTIL_TraceSpline(FHitResult& t, const FVector& start, FVector direction,
    FVector force, uint16 maxIterations = 4096,
    SLineDrawParams* rendered = NULL,
    AActor** ppIgnoredActors = NULL);

/*
 * Given a start position, end position, color, thickness, and lifetime,
 * draws a line in the world.
 */
void UTIL_DrawLine(FVector start, FVector end, SLineDrawParams* rendered);

/*
 * Given a start position, end position, force, color, thickness,
 * and lifetime, draws a spline curve in the world.
 * The direction and magnitude of force determines the curvature of the spline.
 */
void UTIL_DrawSpline(FVector start, FVector end, FVector force, FColor c,
    float thickness, ftime life);

/*
 * Given a start position, radius, and render parameters,
 * renders a circle in the world.
 */
void UTIL_DrawCircle(FVector loc, vec radius, SLineDrawParams*);

/*
 * Given a vector vDirection originating from vOrigin, finds the distance
 * between that worldline and the given point.
 * The magnitude of vDirection does not effect the returned value.
 */
vec UTIL_DistanceToLine(FVector vDirection, FVector vOrigin, FVector point);