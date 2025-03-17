#pragma once
#include "CoreMinimal.h"

class ABGPlayer;

//Given a string, returns a player whose name matches the string
void GetPlayersFromString(TArray<ABGPlayer*>& outList, const char* pszString, ABGPlayer* pRequester = nullptr);

//From the given inList, finds players within the given radius from loc, putting the result into outList
void GetPlayersInRadius(const TArray<ABGPlayer*>& inList, TArray<ABGPlayer*>& outList, FVector loc, float radius);