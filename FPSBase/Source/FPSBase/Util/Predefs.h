#pragma once

#include "CoreMinimal.h"

#ifndef FPSBASE_API
#define FPSBASE_API
#endif

class FString;

typedef unsigned char		byte;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned int		bf;
typedef unsigned short		uint16;
typedef unsigned long long	ulong;
typedef float				ftime; // time
typedef float				vec;   // float representing world space (used by vectors)
typedef unsigned short		eindex; // entity index
typedef float				lerp;   // linear interpolation


//Each language has a language identifier, ex. "en", "es"
typedef FString LangCode;

//Each faction can override the names and descriptions of various things
//to fit their faction, ex. Alibaba piece being called Optio in Roman faction
typedef FString FactionLangFlavor;

#define STRINGIZE(tokens) #tokens // stringifies a token

#define until(condition) while (!(condition)) // simplifies thinking in many contexts
//#define INTERFACE        class // abstract class never directly constructed

#define BF(shiftAmount) (1 << shiftAmount)

FORCEINLINE bool CharIsNumber(TCHAR c) { return (c <= '9' && c >= '0'); }

#define FCOLOR_WHITE FColor(255, 255, 255)
#define FCOLOR_RED FColor(255, 0, 0)
#define FCOLOR_TAN FColor(232, 231, 182)
#define FCOLOR_TAN_DARK FColor(129, 126, 90)