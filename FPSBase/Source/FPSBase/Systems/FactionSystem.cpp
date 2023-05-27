#include "FactionSystem.h"
#include "../Profiles/PlayerClassProfile.h"

#define DEF_FACTION()

//Faction Definitions
/*Faction g_AmericanClassic = Faction(
	FN_AmericanClassic,
	"amer_classic",
	FString("#Faction_American_Name"),
	FColor(153, 204, 255, 255),
	{
		PlayerClasses::g_pAInfantry,
		PlayerClasses::g_pAOfficer,
		PlayerClasses::g_pAFrontiersman,
		PlayerClasses::g_pAMilitia,
		PlayerClasses::g_pAFrenchGre
	}
);*/

TArray<Faction*> Faction::s_allFactions;

void Faction::FactionSwap(Faction** ppTarget, Faction* pNewFaction) {
	Faction* pOldFaction = *ppTarget;

	//TODO swap players to their new classes

	//actually set the faction pointer
	*ppTarget = pNewFaction;

	Msg("Faction %s has changed to %s", CStr(pOldFaction->GetDisplayName()), CStr(pNewFaction->GetDisplayName()));
}

Faction::Faction() {

	s_allFactions.Emplace(this);

	JT_START_BINDING("faction", Faction);
	JT_BIND_INT(m_factionNumber, "factionNumber", true);
	JT_BIND_STRING(m_sDisplayName, "displayName", true);
	JT_BIND_OBJECT(m_cColor, "color", true);
	JT_BIND_STRING_ARRAY(m_aIncludedClasses, "includedClasses", true);
}

const TArray<Faction*>* Faction::GetAllFactions() {
	return &s_allFactions;
}

Faction* g_FactionAttacker = NULL; // Factions::g_AmericanClassic;
Faction* g_FactionDefender = NULL; // Factions::g_AmericanClassic;
Faction* g_FactionBystander = NULL; // Factions::g_CivilianSpectator;
