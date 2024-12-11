#pragma once

class ABGPlayer;

//Given a string, returns a player whose name matches the string
void GetPlayersFromString(TArray<ABGPlayer*>& outList, const char* pszString, ABGPlayer* pRequester = nullptr);
