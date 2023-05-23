#include "FactionSystem.h"
#include "../Profiles/PlayerClassProfile.h"

const PlayerClass* classes[] = {
	PlayerClasses::g_pAInfantry,
	PlayerClasses::g_pAOfficer,
	PlayerClasses::g_pAFrontiersman,
	PlayerClasses::g_pAMilitia,
	PlayerClasses::g_pAFrenchGre
};

//Faction Definitions
Faction g_AmericanClassic = Faction(
	FN_AmericanClassic,
	"amer_classic",
	FString("#Faction_American_Name"),
	FColor(153, 204, 255, 255),
	classes
);


void Faction::FactionSwap(Faction** ppTarget, Faction* pNewFaction) {

}

Faction::Faction(
	EFactionNumber eNumber,
	const char* codeName,
	const FString& displayName,
	FColor factionColor,
	const PlayerClass* classes[]) {

	s_allFactions.Emplace(this);
	m_factionNumber = eNumber;
	m_pszCodeName = codeName;
	m_sDisplayName = displayName;

}


Faction* g_FactionAttacker = Factions::g_AmericanClassic;
Faction* g_FactionDefender = Factions::g_AmericanClassic;
Faction* g_FactionBystander = Factions::g_Civilian;
