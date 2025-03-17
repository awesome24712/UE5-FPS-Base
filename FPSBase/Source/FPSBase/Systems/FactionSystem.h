#pragma once
#include "CoreMinimal.h"
#include "ConsoleSystem.h"
#include "../PODT/BColor.h"
#include "../Util.h"

class PlayerClassProfile;
class ABGPlayer;
class Faction;

extern Faction* g_FactionAttacker;
extern Faction* g_FactionDefender;
extern Faction* g_FactionBystander;

enum EFactionNumber : uint8 {
	FN_AmericanClassic,
	FN_BritishClassic,
	FN_EnglishAmerican,
	FN_EnglishBritish,
	FN_French,
	FN_AmericanIndigenous,
	FN_BritishIndigenous,
	FN_Hessian,
	FN_Civilian,
};

inline ETeamNumber GetOtherTeam(ETeamNumber t) {
	return t == TEAM_ATTACKER ? TEAM_DEFENDER : TEAM_ATTACKER;
}

//General iteratorables for players
const TArray<ABGPlayer*>& GetPlayers();
const TArray<ABGPlayer*>& GetAttackingPlayers();
const TArray<ABGPlayer*>& GetDefendingPlayers();
const TArray<ABGPlayer*>& GetBystandingPlayers();
const TArray<ABGPlayer*>& GetTeammatesForPlayer(ABGPlayer* pPlayer);
const TArray<ABGPlayer*>& GetEmptyPlayerArray();

inline const TArray<ABGPlayer*>& GetPlayersOfTeam(ETeamNumber t) {
	if (t >= NUM_TEAMS) {
		NLogger::Fatal("FactionFromTeamNumber BAD number");
	}

	const TArray<ABGPlayer*>& (*ptrs[5])() = {
		&GetEmptyPlayerArray,
		&GetAttackingPlayers,
		&GetDefendingPlayers,
		&GetBystandingPlayers,
		&GetEmptyPlayerArray
	};

	return ptrs[t]();
}

Faction* FactionFromTeamNumber(ETeamNumber team);

//returns NULL for bystanders
Faction* EnemyFactionFromTeamNumber(ETeamNumber team);

//Faction system - note that faction != team, i.e. different factions can be assigned to different teams
class Faction : public IJsonBindable {
private:
	EFactionNumber m_factionNumber; //shorthand identifier for faction

	//const char* m_pszCodeName; //name as it appears in console and logs, unlocalized
	FString m_sDisplayName; //name as to be shown to users in generic cases, localized
	FString m_sAbbreviation;
	BColor m_cColor; //color representation for this team
	TArray<FString> m_aIncludedClasses; //array of class codeNames to load into m_classes

	TArray<const PlayerClassProfile*> m_classes;
	TArray<ABGPlayer*> m_players;

	static TArray<Faction*> s_allFactions;
	static void FactionSwap(Faction** ppTarget, Faction* pNewFaction);

	static bool FactionChangeAllowedByAutoabalance(ABGPlayer* pChanger, Faction* pNewFaction);

public:
	Faction();

	//Called when player joins server, assigns them to unassigned team
	static Faction* NotifyPlayerJoined(ABGPlayer* pPlayer);

	//Called when player changes teams, including the first time, can fail
	//returns NULL if failed
	static Faction* RequestPlayerFactionChange(ABGPlayer* pPlayer, ETeamNumber tn);

	//Called to force player team change
	static Faction* AssignPlayerToFaction(ABGPlayer* pPlayer, ETeamNumber tn);

	//Called when a player leaves the server
	static void NotifyPlayerLeft(ABGPlayer* pPlayer);

	//Call this infrequently to change player teams as needed
	static void AutobalanceThink();

	static void SetAttacker(Faction* pFaction) { FactionSwap(&g_FactionAttacker, pFaction); };
	static void SetDefender(Faction* pFaction) { FactionSwap(&g_FactionDefender, pFaction); };
	static void SetBystander(Faction* pFaction) { FactionSwap(&g_FactionBystander, pFaction); };

	EFactionNumber GetFactionNumber() const { return m_factionNumber; }
	const FString& GetDisplayName() const { return m_sDisplayName; }
	const FColor& GetColor() const { return m_cColor; }

	const TArray<ABGPlayer*>& GetPlayers() const { return m_players; }
	int GetNumPlayers() const { return m_players.Num(); }

	//-- Global faction handling functions
	static const TArray<Faction*>* GetAllFactions();

	static EFactionNumber GetAttackerNum();
	static EFactionNumber GetDefenderNum();
	static EFactionNumber GetBystanderNum();

};

Faction* GetAttacker();
Faction* GetDefender();
Faction* GetBystander();

extern Faction* g_FactionAttacker;
extern Faction* g_FactionDefender;
extern Faction* g_FactionBystander;

extern ConVar gr_friendlyfire;
extern ConVar gr_friendlyfire_grenades;
extern ConVar gr_friendlyfire_cannon;