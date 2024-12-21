#include "WeaponDefProfile.h"
#include "../Util.h"

TArray<WeaponDef*> g_allWeapons;

WeaponAttackInfo::WeaponAttackInfo() {

	//Bindings
	JT_START_BINDING("weaponAttackInfo", WeaponAttackInfo);
	JT_BIND_BYTE(m_iAttacktype, "attackType", true);
	JT_BIND_BOOLEAN(m_bFiresUnderwater, "firesUnderwater", true);
	JT_BIND_FLOAT(m_flRange, "range", true);
	JT_BIND_INT(m_iDamage, "damage", true);
	JT_BIND_FLOAT(m_flRecoil, "recoil", false);
	JT_BIND_FLOAT(m_flCosAngleTolerance, "cosAngleTolerance", false);
	JT_BIND_FLOAT(m_flRetraceDuration, "retraceDuration", false);
	JT_BIND_FLOAT(m_flRetraceDelay, "flRetraceDelay", false);
	JT_BIND_FLOAT(m_flAttackrate, "attackRate", false);
	JT_BIND_INT(m_iStaminaDrain, "staminaDrain", true);
	//accuracies
	JT_BIND_FLOAT(m_flBaseAccuracy, "baseAccuracy", false);

	JT_BIND_FLOAT(m_flConstantDamageRange, "constantDamageRange", false);
	JT_BIND_FLOAT(m_flRelativeDrag, "dragMultiplier", false);
	JT_FINISH_BINDING();
}

//Maps weapon names to their weapon defs - for external usage
TMap<FName, WeaponDef*> g_dictWeaponDefs;

//-----------------------------------------------------------------------------
// Purpose: Puts any weapon def into the dictionary
//-----------------------------------------------------------------------------
WeaponDef::WeaponDef() {
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

	JT_START_BINDING("WeaponDef", WeaponDef);
	JT_BIND_OBJECT(m_Attackinfos[0], "attack1", true);
	JT_BIND_OBJECT(m_Attackinfos[1], "attack2", true);
	JT_BIND_FLOAT(m_flShotAimModifier, "shotAimModifier", false);
	JT_BIND_FLOAT(m_flShotSpread, "shotSpread", false);
	JT_BIND_BYTE(m_iNumShot, "numShot", false);
	JT_BIND_INT(m_iDamagePerShot, "damagePerShot", false);
	JT_BIND_BOOLEAN(m_bShotOnly, "shotOnly", false);
	JT_BIND_FLOAT(m_flShotMuzzleVelocity, "shotMuzzleVelocity", false);
	JT_BIND_FLOAT(m_flMuzzleVelocity, "muzzleVelocity", false);
	JT_BIND_FLOAT(m_flDamageDropoffMultiplier, "damageDropoffMultiplier", false);
	JT_BIND_BOOLEAN(m_bPenetrateFlesh, "penetrateFlesh", false);
	JT_BIND_FLOAT(m_flVerticalAccuracyScale, "verticalAccuracyScale", false);
	JT_BIND_FLOAT(m_flZeroRange, "zeroRange", false);
	JT_BIND_BYTE(m_iOwnerSpeedModOnKill, "speedModOnKill", false);
	JT_BIND_BYTE(m_iAerialDamageMod, "aerialDamageMod", false);
	JT_BIND_INT(m_iExtraDamageTypes, "extraDamageTypes", false);
	JT_BIND_BOOLEAN(m_bDontAutoreload, "dontAutoReload", false);
	JT_BIND_BOOLEAN(m_bCantAbortReload, "cantAbortReload", false);
	JT_BIND_FLOAT(m_flReloadMovementSpeedModifier, "reloadMovementSpeedModifier", false);
	JT_BIND_FLOAT(m_flIronsightFOVOffset, "ironsightFOV_Offset", false);
	JT_BIND_BOOLEAN(m_bWeaponHasSights, "weaponHasSights", true);
	JT_BIND_BOOLEAN(m_bDemoteNonHeadhitsToSecondaryDamage, "demoteHeadHitsToSecondaryDamage", false);
	JT_BIND_FLOAT(m_flIronsightsTime, "ironsightsTime", false);
	JT_BIND_BOOLEAN(m_bBreakable, "breakable", false);
	JT_BIND_FLOAT(m_fHolsterTime, "holsterTime", true);
	JT_BIND_FLOAT(m_flLockTime, "lockTime", false);
	JT_BIND_FLOAT(m_flRandomAdditionalLockTimeMax, "randomAdditionalLockTime", false);
	//JT_BIND_STRING(m_weaponName, "weaponName", true);
	JT_FINISH_BINDING();

}

//Default weapon def used by non-BG3 weapons
//DEC_BG3_WEAPON_DEF(weapon_default); //Use this to declare a weapon definiton
//DEF_BG3_WEAPON_DEF(weapon_default) {} //Use this to define a weapon definitions constructor

//-----------------------------------------------------------------------------
// Purpose: Gets default weapon def for weapons
//-----------------------------------------------------------------------------
const WeaponDef* WeaponDef::GetDefault() {
	//return &g_Def_weapon_default;
	return NULL;
}

void WeaponDef::LoadWeaponProfilesFromDisk() {
	NTreeManager::LoadBindablesFromFolder(g_allWeapons, "Weapons");
}

const TArray<WeaponDef*>& WeaponDef::GetAllWeapons() {
	return g_allWeapons;
}

//-----------------------------------------------------------------------------
// Purpose: Given a weapon name, returns the weapon def used by it.
//		For external usage; instantiated weapons should get their weapon def in
//		their own constructors.
//-----------------------------------------------------------------------------
const WeaponDef* WeaponDef::GetDefForWeapon(const FName& weaponName) {
	return g_dictWeaponDefs[weaponName];
}


CON_COMMAND(weapon_list, 0, "Lists all standard BG3 weapons, excluding grenades") {
	for (auto pair : g_dictWeaponDefs) {
		Msg("%s\n", WCStr(pair.Value->GetCodeName()));
	}
}