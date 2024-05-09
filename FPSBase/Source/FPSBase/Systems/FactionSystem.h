#pragma once
#include "CoreMinimal.h"
#include "ConsoleSystem.h"
#include "../PODT/BColor.h"

class PlayerClass;
class Faction;

extern Faction* g_FactionAttacker;
extern Faction* g_FactionDefender;
extern Faction* g_FactionBystander;

enum EFactionNumber : uint8 {
	FN_AmericanClassic,
	FN_BritishClassic,
	FN_AmericanPure,
	FN_BritishPure,
	FN_French,
	FN_AmericanNative,
	FN_BritishNative,
	FN_Hessian,
	FN_Civilian,
};

//Faction system - note that faction != team, i.e. different factions can be assigned to different teams
class Faction : public IJsonBindable {
private:
	EFactionNumber m_factionNumber; //shorthand identifier for faction

	const char* m_pszCodeName; //name as it appears in console and logs, unlocalized
	FString m_sDisplayName; //name as to be shown to users in generic cases, localized
	BColor m_cColor; //color representation for this team
	TArray<FString> m_aIncludedClasses; //array of class codeNames to load into m_pClasses

	TArray<const PlayerClass*> m_pClasses;

	static TArray<Faction*> s_allFactions;
	static void FactionSwap(Faction** ppTarget, Faction* pNewFaction);

public:
	Faction();

	EFactionNumber GetFactionNumber() const { return m_factionNumber; }
	const FString& GetDisplayName() const { return m_sDisplayName; }
	const FColor& GetColor() const { return m_cColor; }

	//-- Global faction handling functions
	static const TArray<Faction*>* GetAllFactions();
	static void SetAttacker(Faction* pFaction) { FactionSwap(&g_FactionAttacker, pFaction); };
	static void SetDefender(Faction* pFaction) { FactionSwap(&g_FactionDefender, pFaction); };
	static void SetBystander(Faction* pFaction) { FactionSwap(&g_FactionBystander, pFaction); };

	static EFactionNumber GetAttackerNum();
	static EFactionNumber GetDefenderNum();
	static EFactionNumber GetBystanderNum();

	static Faction* GetAttacker();
	static Faction* GetDefender();
	static Faction* GetBystander();



};

/*namespace Factions {
	//Faction declarations
#define DEC_BG4_FACTION(name) \
	extern Faction* g_##name;

	DEC_BG4_FACTION(AmericanClassic);
	DEC_BG4_FACTION(BritishClassic);
	DEC_BG4_FACTION(AmericanPure);
	DEC_BG4_FACTION(BritishPure);
	DEC_BG4_FACTION(French);
	DEC_BG4_FACTION(AmericanNative);
	DEC_BG4_FACTION(BritishNative);
	DEC_BG4_FACTION(Hessian);
	DEC_BG4_FACTION(CivilianSpectator);

}*/

extern Faction* g_FactionAttacker;
extern Faction* g_FactionDefender;
extern Faction* g_FactionBystander;

extern ConVar gr_friendlyfire;
extern ConVar gr_friendlyfire_grenades;
extern ConVar gr_friendlyfire_cannon;