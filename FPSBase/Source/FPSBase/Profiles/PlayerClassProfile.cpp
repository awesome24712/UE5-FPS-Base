#include "PlayerClassProfile.h"

ConVar mp_year("mp_year", CVAR_MOD | CVAR_NOTIFY, 1779, "Year setting controls which weapons are available, for historical accuracy. Used only if mp_year_accuracy = 1");
ConVar mp_year_accuracy("mp_year_accuracy", CVAR_MOD | CVAR_NOTIFY, false, "Controls whether or not to use year-by-year historical weapon accuracy.");

GunKit::GunKit() {
	JT_START_BINDING("gunkit", GunKit);
	JT_BIND_STRING(m_weaponPrimaryName, "weaponPrimaryName", true);
	JT_BIND_BOOLEAN(m_bAllowBuckshot, "allowBuckshot", false);
	JT_BIND_BYTE(m_iAmmoOverrideCount, "ammoOverrideCount", false);
	JT_BIND_BYTE(m_iMovementSpeedModifier, "movementSpeedModifier", false);
	JT_BIND_BYTE(m_iSleeveTextureOverride, "sleeveTextureOverride", false);
	//JT_BIND_STRING(m_ammoOverrideName, "ammoOverrideName", false);
	JT_BIND_USHORT(m_iMinYear, "minYear", true);
	JT_BIND_USHORT(m_iMaxYear, "maxYear", true);
	JT_FINISH_BINDING();
}

bool GunKit::AvailableForCurrentYear() const {
	//return true;
	if (mp_year_accuracy.GetValueBool() && mp_year.GetValueBool()) {
		int year = mp_year.GetValueInt();
		return (year >= m_iMinYear && year <= m_iMaxYear);
	}
	else {
		return !m_bDisabledOnHistoricityDisabled || mp_year.GetValueInt() == 0;
	}
}

PlayerClass::PlayerClass() {
	//TODO add population limits!
	//m_pPopCounter = new NClassQuota::SPopulationCounter;
	// 
	// 
	//memset(m_aUniformControllingBits, 0, sizeof(m_aUniformControllingBits));
	//memset(m_aUniformControllingBitsActive, 0, sizeof(m_aUniformControllingBitsActive));
	//memset(m_pszUniformModelOverrides, 0, sizeof(m_pszUniformModelOverrides));
	/*for (int i = 0; i < NUM_POSSIBLE_UNIFORMS; i++) {
		m_aUniformSleeveOverrides[i] = m_aUniformArmModelOverrides[i] = -1;
	}*/

	JT_START_BINDING("playerclass", PlayerClass);
	JT_BIND_BYTE(m_iRoleNumber, "roleNumber", true);
	JT_BIND_BYTE(m_iDefaultPrimaryAmmoCount, "defaultPrimaryAmmoCount", true);
	JT_BIND_BYTE(m_iDefaultSecondaryAmmoCount, "secondaryAmmoCount", false);
	JT_BIND_FLOAT(m_flBaseSpeedOriginal, "baseSpeed", true);
	JT_BIND_FLOAT(m_flFlagWeightMultiplier, "flagWeightMultiplier", true);
	//weapon kits
	JT_BIND_OBJECT(m_aWeapons[0], "kit0", true);
	JT_BIND_OBJECT(m_aWeapons[1], "kit1", false);
	JT_BIND_OBJECT(m_aWeapons[2], "kit2", false);
	JT_BIND_OBJECT(m_aWeapons[3], "kit3", false);
	JT_BIND_OBJECT(m_aWeapons[4], "kit4", false);
	JT_BIND_OBJECT(m_aWeapons[5], "kit5", false);
	JT_BIND_OBJECT(m_aWeapons[6], "kit6", false);
	JT_BIND_OBJECT(m_aWeapons[7], "kit7", false);
	JT_BIND_OBJECT(m_aWeapons[8], "kit8", false);
	JT_BIND_BOOLEAN(m_bDefaultRandomUniform, "defaultRandomUniform", true);
	JT_BIND_BOOLEAN(m_bCanDoVcommBuffs, "canDoVcommBuffs", false);
	JT_BIND_BOOLEAN(m_bHasImplicitDamageResistance, "hasImplicitDamageResistance", true);
	JT_BIND_BOOLEAN(m_bHasImplicitDamageWeakness, "hasImplicitDamageWeakness", true);
	JT_BIND_BOOLEAN(m_bNerfResistance, "nerfResistance", true);
	JT_FINISH_BINDING();
}

void PlayerClass::postClassConstruct(PlayerClass* pClass) {

	pClass->m_flBaseSpeedCalculated = pClass->m_flBaseSpeedOriginal;

	//count number of chooseable weapons
	const GunKit* pKit = pClass->m_aWeapons;
	int chooseable = 0;
	for (int i = 0; i < NUM_POSSIBLE_WEAPON_KITS; i++) {
		if (!pKit[i].m_weaponPrimaryName.IsEmpty())
			chooseable++;
	}
	pClass->m_iChooseableKits = chooseable;
}

void PlayerClass::getWeaponDef(byte iKit, const WeaponDef** ppPrimary, const WeaponDef** ppSecondary, const WeaponDef** ppTertiary) const {
	//we always have a primary
	*ppPrimary = WeaponDef::GetDefForWeapon(m_aWeapons[iKit].m_weaponPrimaryName);
	const FString& second = m_aWeapons[iKit].m_weaponSecondaryName;
	const FString& third = m_aWeapons[iKit].m_weaponTertiaryName;
	if (!second.IsEmpty()) {
		*ppSecondary = WeaponDef::GetDefForWeapon(second);
		if (!third.IsEmpty()) {
			*ppTertiary = WeaponDef::GetDefForWeapon(third);
		}
		else {
			*ppTertiary = nullptr;
		}
	}
	else {
		*ppSecondary = nullptr;
		*ppTertiary = nullptr;
	}
}