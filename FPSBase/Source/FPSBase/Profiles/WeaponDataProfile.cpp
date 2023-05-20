#include "WeaponDataProfile.h"
#include "../Util.h"

//Maps weapon names to their weapon defs - for external usage
TMap<FString, WeaponDef*> g_dictWeaponDefs;

//-----------------------------------------------------------------------------
// Purpose: Puts any weapon def into the dictionary
//-----------------------------------------------------------------------------
WeaponDef::WeaponDef(const char* pszWeaponName) {
	//m_bQuickdraw = false;
	//m_bSlowDraw = false;
	m_flIronsightsTime = 0.3f;
	m_flRandomAdditionalLockTimeMax = 0.0f;

	m_bDontAutoreload = true;
	m_bCantAbortReload = true;
	m_flReloadMovementSpeedModifier = 1.0f;
	m_flDamageDropoffMultiplier = 1.0f;
	m_bPenetrateFlesh = false;
	m_iOwnerSpeedModOnKill = m_iAerialDamageMod = 0;
	m_flVerticalAccuracyScale = 0.5;
	m_bDemoteNonHeadhitsToSecondaryDamage = false;

	//m_eWeaponType = GENERIC;
	m_flLockTime = 0.135f;

	m_flIronsightFOVOffset = 0.0f;
	m_bWeaponHasSights = false;
	m_bBreakable = false;

	m_Attackinfos[0].m_flRetraceDelay = m_Attackinfos[1].m_flRetraceDelay = 0;

	m_bFiresUnderwater = false;
	m_bAltFiresUnderwater = false;

	g_dictWeaponDefs.Add(pszWeaponName, this);
	m_pszWeaponDefName = pszWeaponName;

	m_bShotOnly = false;

	m_iExtraDamageTypes = 0;

#ifdef CLIENT_DLL
	//generate unique color based on weapon name
	int nameSum = 0;
	int len = strlen(pszWeaponName);
	for (int i = 6; i < len; i++) {
		nameSum += ~pszWeaponName[i];
	}
	RndSeed(nameSum);
	int mainColor = RndInt(0, 2);
	int rmin = mainColor == 0 ? 200 : 0;
	int gmin = mainColor == 1 ? 200 : 0;
	int bmin = mainColor == 2 ? 200 : 0;
	m_graphColor = FColor(RndInt(rmin, 255), RndInt(gmin, 255), RndInt(bmin, 255), 255);
#endif
}

//Default weapon def used by non-BG3 weapons
DEC_BG3_WEAPON_DEF(weapon_default); //Use this to declare a weapon definiton
DEF_BG3_WEAPON_DEF(weapon_default) {} //Use this to define a weapon definitions constructor

//-----------------------------------------------------------------------------
// Purpose: Gets default weapon def for weapons
//-----------------------------------------------------------------------------
const WeaponDef* WeaponDef::GetDefault() {
	return &g_Def_weapon_default;
}

//-----------------------------------------------------------------------------
// Purpose: Given a weapon name, returns the weapon def used by it.
//		For external usage; instantiated weapons should get their weapon def in
//		their own constructors.
//-----------------------------------------------------------------------------
const WeaponDef* WeaponDef::GetDefForWeapon(const char* pszWeaponName) {
	return g_dictWeaponDefs[FString(pszWeaponName)];
}


CON_COMMAND(weapon_list, 0, "Lists all standard BG3 weapons, excluding grenades") {
	for (auto pair : g_dictWeaponDefs) {
		Msg("%s\n", pair.Value->m_pszWeaponDefName);
	}
}