#include "FactionSystem.h"
#include "../Profiles/PlayerClassProfile.h"
#include "../BGPlayer.h"
#include "../Util.h"

#define CVAR_FLAGS (CVAR_MOD | CVAR_REPLICATED | CVAR_NOTIFY)
ConVar sr_autobalance("sr_autobalance", CVAR_FLAGS, 1, "Whether or not autobalance is on and what the difference limit is. 0 to disable.");

ConVar sr_faction_attackers("sr_faction_attackers", CVAR_FLAGS, FN_AmericanClassic, "Default attacker faction");
ConVar sr_faction_defenders("sr_faction_defenders", CVAR_FLAGS, FN_BritishClassic, "Default attacker faction");
ConVar sr_faction_bystanders("sr_faction_bystanders", CVAR_FLAGS, FN_Civilian, "Default attacker faction");

Faction* g_FactionAttacker = NULL;
Faction* g_FactionDefender = NULL;
Faction* g_FactionBystander = NULL;

static TArray<ABGPlayer*> g_allPlayers;
const TArray<ABGPlayer*>& GetPlayers() {
	return g_allPlayers;
}

const TArray<ABGPlayer*>& GetAttackingPlayers() {
	return Faction::GetAttacker()->GetPlayers();
}

const TArray<ABGPlayer*>& GetDefendingPlayers() {
	return Faction::GetDefender()->GetPlayers();
}

const TArray<ABGPlayer*>& GetBystandingPlayers() {
	return Faction::GetBystander()->GetPlayers();
}

const TArray<ABGPlayer*>& GetTeammatesForPlayer(ABGPlayer* pPlayer) {
	return pPlayer->GetFaction()->GetPlayers();
}

Faction* FactionFromTeamNumber(ETeamNumber team) {
	if (team >= NUM_TEAMS) {
		NLogger::Fatal("FactionFromTeamNumber BAD number");
	}

	Faction* factions[] = {
		g_FactionAttacker,
		g_FactionDefender,
		g_FactionBystander,
		g_FactionBystander,
	};

	return factions[team];
}

Faction* EnemyFactionFromTeamNumber(ETeamNumber team) {
	if (team >= NUM_TEAMS) {
		NLogger::Fatal("EnemyFactionFromTeamNumber BAD number");
		return NULL;
	}

	Faction* factions[] = {
		g_FactionDefender,
		g_FactionAttacker,
		NULL,
		NULL,
	};

	return factions[team];
}

TArray<Faction*> Faction::s_allFactions;

void Faction::FactionSwap(Faction** ppTarget, Faction* pNewFaction) {
	Faction* pOldFaction = *ppTarget;

	//TODO swap players to their new classes

	//actually set the faction pointer
	*ppTarget = pNewFaction;

	Msg("Faction %s has changed to %s", NAME_TO_ANSI(pOldFaction->GetDisplayName()), NAME_TO_ANSI(pNewFaction->GetDisplayName()));
}


bool Faction::FactionChangeAllowedByAutoabalance(ABGPlayer* pChanger, Faction* pNewFaction) {

	int maxDifference = sr_autobalance.GetValueInt();

	//always allow change to spectator
	//or when autobalance is disabled
	if (maxDifference == 0 || pNewFaction == GetBystander()) {
		return true;
	}

	//friendly is the team we're wanting to join, enemy is the other team
	Faction& friendly = *pNewFaction;
	Faction& enemy = *(pNewFaction == GetAttacker() ? GetDefender() : GetAttacker());

	int friendlyCount = friendly.GetNumPlayers() + 1; //add one because we're joining
	int enemyCount = enemy.GetNumPlayers();
	//subtract 1 if we're leaving the enemy
	if (&enemy == pChanger->GetFaction()) {
		enemyCount -= 1;
	}

	//now compare
	return abs(friendlyCount - enemyCount) <= maxDifference;
}


Faction* Faction::NotifyPlayerJoined(ABGPlayer* pPlayer) {
	pPlayer->m_iTeam = TEAM_UNASSIGNED;
	GetBystander()->m_players.Add(pPlayer);
	g_allPlayers.Add(pPlayer);
	return GetBystander();
}

Faction* Faction::RequestPlayerFactionChange(ABGPlayer* pPlayer, ETeamNumber tn) {
	//do nothing if they're already on that team
	if (pPlayer->m_iTeam == tn) {
		return NULL;
	}

	Faction* pNewFaction = FactionFromTeamNumber(tn);
	if (!pNewFaction) {
		NLogger::Fatal("Player requested invalid team");
		return NULL;
	}

	if (!FactionChangeAllowedByAutoabalance(pPlayer, pNewFaction)) {
		Msg("Faction change blocked by autobalance");
		return NULL;
	}

	return AssignPlayerToFaction(pPlayer, tn);
}

Faction* Faction::AssignPlayerToFaction(ABGPlayer* pPlayer, ETeamNumber tn) {
	Faction* pOldFaction = pPlayer->GetFaction();
	Faction* pNewFaction = FactionFromTeamNumber(tn);

	//remove player from old faction and add to new
	pPlayer->m_iTeam = tn;
	pOldFaction->m_players.Remove(pPlayer);
	pNewFaction->m_players.Add(pPlayer);

	Msg("Player %s has changed to team %s", WCStr(pPlayer->GetPlayerName()), NAME_TO_ANSI(pNewFaction->GetDisplayName()));

	return pNewFaction;
}

void Faction::NotifyPlayerLeft(ABGPlayer* pPlayer) {
	g_allPlayers.Remove(pPlayer);
}

Faction::Faction() {

	s_allFactions.Emplace(this);

	JT_START_BINDING("faction", Faction);
	JT_BIND_INT(m_factionNumber, "factionNumber", true);
	JT_BIND_NAME(m_sDisplayName, "displayName", true);
	JT_BIND_OBJECT(m_cColor, "color", true);
	JT_BIND_NAME_ARRAY(m_aIncludedClasses, "includedClasses", true);
	JT_FINISH_BINDING_WITH_CALLBACK([](IJsonBindable* pObj) {
		//assign to globals if they aren't assigned already
		Faction* pFaction = (Faction*) pObj;
		int attacker = sr_faction_attackers.GetValueInt();
		int defender = sr_faction_defenders.GetValueInt();
		int bystander = sr_faction_bystanders.GetValueInt();

		if (pFaction->m_factionNumber == attacker) {
			g_FactionAttacker = pFaction;
		}
		else if (pFaction->m_factionNumber == defender) {
			g_FactionDefender = pFaction;
		}
		else if (pFaction->m_factionNumber == bystander) {
			g_FactionBystander = pFaction;
		}
	});
}

const TArray<Faction*>* Faction::GetAllFactions() {
	return &s_allFactions;
}


EFactionNumber Faction::GetAttackerNum() {
	return g_FactionAttacker->GetFactionNumber();
}
EFactionNumber Faction::GetDefenderNum() {
	return g_FactionDefender->GetFactionNumber();
}
EFactionNumber Faction::GetBystanderNum() {
	return g_FactionBystander->GetFactionNumber();
}

Faction* Faction::GetAttacker() {
	return g_FactionAttacker;
}
Faction* Faction::GetDefender() {
	return g_FactionDefender;
}
Faction* Faction::GetBystander() {
	return g_FactionBystander;
}

