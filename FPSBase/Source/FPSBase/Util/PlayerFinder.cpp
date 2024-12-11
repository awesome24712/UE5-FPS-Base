
#include "PlayerFinder.h"
#include "CoreMinimal.h"
#include "../BGPlayer.h"
#include "../Systems/FactionSystem.h"


inline int StringMatchSimple(const char* pszFirst, const char* pszSecond) {
	int match = 0;
	while (pszFirst[match] && pszSecond[match] && pszFirst[match] == pszSecond[match]) {
		match++;
	}
	return match;
}

static int GetStringMatchLength(FString str, FString searchTerm, bool* bExactMatch, bool bIgnoreCase = false) {
	size_t maxMatchLength = 0;
	*bExactMatch = false;

	//only bother searching if it's feasible
	if (str.Len() >= searchTerm.Len() && searchTerm.Len() > 0) {

		//ignore case if necessary
		if (bIgnoreCase) {
			for (size_t i = 0; i < str.Len(); i++) {
				str[i] = tolower(str[i]);
			}
			for (size_t i = 0; i < searchTerm.Len(); i++) {
				searchTerm[i] = tolower(searchTerm[i]);
			}
		}

		//find longest match
		for (size_t i = 0; i < str.Len(); i++) {
			size_t match = StringMatchSimple(CStr(str) + i, CStr(searchTerm));
			if (match > maxMatchLength) {
				maxMatchLength = match;
			}
		}
		*bExactMatch = maxMatchLength == str.Len();
	}
	return maxMatchLength;
}

ABGPlayer* FindPlayerByName(const char* pszName) {
	ABGPlayer* pResult = NULL;
	int maxMatchLength = 0;
	bool bExactMatch = false;

	//first search for players, not ignoring case
	for (auto pPlayer : GetPlayers()) {
		if (pPlayer) {
			//find longest match
			int matchLength = GetStringMatchLength(pPlayer->GetPlayerName(), pszName, &bExactMatch);
			//Msg("%i, %s == %s\n", matchLength, pPlayer->GetPlayerName(), pszName);
			if (matchLength >= maxMatchLength) {
				maxMatchLength = matchLength;
				pResult = pPlayer;
			}

			//if we've found exact match, quit loop now
			if (bExactMatch) {
				break;
			}
		}
	}

	//check if we need to more freely ignore case
	if (!bExactMatch && maxMatchLength < 4) {
		for (auto pPlayer : GetPlayers()) {
			if (pPlayer) {

				//find longest match, ignoring case
				int matchLength = GetStringMatchLength(pPlayer->GetPlayerName(), pszName, &bExactMatch, true);
				if (matchLength > maxMatchLength) {
					maxMatchLength = matchLength;
					pResult = pPlayer;
				}

				//if we've found exact match, quit loop now
				if (bExactMatch) {
					break;
				}
			}
		}
	}

	//if we don't have exact match and match is short, ignore it
	if (!bExactMatch && maxMatchLength < 4) {
		pResult = NULL;
	}

	return pResult;
}

void GetPlayersFromString(TArray<ABGPlayer*>& outList, const char* pszString, ABGPlayer* pRequester) {

	outList = TArray<ABGPlayer*>();

	//two possibilities - keyword or name
	if (pszString[0] == '@') {
		//@me
		if (strcmp(pszString, "@me") == 0 && pRequester) {
			outList.Push(pRequester);
		}
		else if (strcmp(pszString, "@aim") == 0 && pRequester) {
			//trace from requester's aim
			FVector aimDirection = pRequester->GetAimDirection();

			FHitResult t;
			FVector eyes;
			FRotator eyesRotation;
			pRequester->GetActorEyesViewPoint(eyes, eyesRotation);
			AActor* ignored[2] = {pRequester, nullptr};
			UTIL_TraceLine(t, eyes, eyes + aimDirection * 2048, ignored);
			
			if (t.GetActor() && ToPlayer(t.GetActor())) {
				outList.Push(ToPlayer(t.GetActor()));
			}
		}
		else {
			bool(*pFunc)(ABGPlayer*) = NULL;
			//@all
			if (strcmp(pszString, "@all") == 0) {
				pFunc = [](ABGPlayer* pPlayer) { return true; };
			}

			//@att
			else if (strcmp(pszString, "@att") == 0) {
				pFunc = [](ABGPlayer* pPlayer) { return pPlayer->GetFaction() == GetAttacker(); };
			}

			//@def
			else if (strcmp(pszString, "@def") == 0) {
				pFunc = [](ABGPlayer* pPlayer) { return pPlayer->GetFaction() == GetDefender(); };
			}

			//@spec
			else if (strcmp(pszString, "@bys") == 0) {
				pFunc = [](ABGPlayer* pPlayer) { return pPlayer->GetFaction() == GetBystander(); };
			}

			//@bot
			else if (strcmp(pszString, "@bot") == 0) {
				pFunc = [](ABGPlayer* pPlayer) { return pPlayer->IsBot(); };
			}

			//@human
			else if (strcmp(pszString, "@human") == 0) {
				pFunc = [](ABGPlayer* pPlayer) { return !pPlayer->IsBot(); };
			}

			//@dead
			else if (strcmp(pszString, "@dead") == 0) {
				pFunc = [](ABGPlayer* pPlayer) { return pPlayer->IsDead(); };
			}

			//@alive
			else if (strcmp(pszString, "@alive") == 0) {
				pFunc = [](ABGPlayer* pPlayer) { return pPlayer->IsAlive(); };
			}

			//add all players who pass the test
			if (pFunc) {
				int index = 0;
				for (auto pPlayer : GetPlayers()) {
					if (pPlayer) {
						if (pFunc(pPlayer)) {
							outList.Push(pPlayer);
						}
					}
				}
			}
		}
	}
	else {
		ABGPlayer* pFoundPlayer = NULL;

		//find player by name or by index
		if (pszString[0] != '\0') {
			pFoundPlayer = FindPlayerByName(pszString);
		}

		outList.Push(pFoundPlayer);
	}
}

CON_COMMAND(psearch, 0, "Finds a player") {
	TArray<ABGPlayer*> players;

	GetPlayersFromString(players, args[1], NULL);
	for (auto p : players) {
		Msg("%s\n", CStr(p->GetPlayerName()));
	}
}


//helper for getting just 1 player from a selector
//returns NULL if not found or if multiple found
ABGPlayer* GetSinglePlayerFromSelector(const char* pszSelector) {
	//no @ selectors
	if (pszSelector[0] == '@')
		return NULL;

	TArray<ABGPlayer*> players;
	GetPlayersFromString(players, pszSelector, NULL);

	ABGPlayer* pResult = NULL;

	//no more than 1
	if (players.Num() == 1) {
		pResult = players[0];
	}

	return pResult;
}