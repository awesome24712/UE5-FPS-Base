#include "KitAccessoryProfile.h"
#include "WeaponDefProfile.h"

KitAccessoryModifiers::KitAccessoryModifiers() {
	//general
	JT_START_BINDING("kitAccessoryModifiers", KitAccessoryModifiers);
	JT_BIND_INT(m_iUniqueFlags, "uniqueFlags", false);

	JT_BIND_FLOAT(m_flWeight, "weight", false);
	JT_BIND_FLOAT(m_flSpeedModifier, "speedModifier", false);
	JT_BIND_FLOAT(m_flMaxWeightModifier, "maxWeightModifier", false);
	JT_BIND_FLOAT(m_flWeightReductionHolstered, "weightReductionHolstered", false);
	JT_BIND_FLOAT(m_flStaminaDrainMultiplier, "staminaDrainMultiplier", false);
	JT_BIND_FLOAT(m_flDamageReceivedMultiplier, "damageReceivedMultiplier", false);
	JT_BIND_FLOAT(m_flLowDamageReceivedMultiplier, "lowDamageReceivedMultiplier", false);

	//shooting
	JT_BIND_FLOAT(m_flLockTimeModifier, "lockTimeModifier", false);
	JT_BIND_FLOAT(m_flAccuracyModifier, "accuracyModifier", false);
	JT_BIND_FLOAT(m_flRecoilModifier, "recoilModifier", false);
	JT_BIND_FLOAT(m_flReloadMovementSpeedModifier, "reloadMovementSpeedModifier", false);
	JT_BIND_FLOAT(m_flReloadSpeedModifier, "reloadSpeedModifier", false);

	//melee
	JT_BIND_FLOAT(m_flMeleeIntervalModifier, "meleeIntervalModifier", false);
	JT_BIND_FLOAT(m_iMeleeRangeExtension, "meleeRangeExtension", false);

	//other
	JT_BIND_FLOAT(m_flMeleeIntervalModifier, "meleeIntervalModifier", false);
	JT_BIND_FLOAT(m_iMeleeRangeExtension, "meleeRangeExtension", false);

	JT_FINISH_BINDING();
}

KitAccessoryProfile::KitAccessoryProfile() {
	m_eType = EAccessoryType::CLASS_PERK;
	m_eFilter = 0;

	memset(&m_modifiers, 0, sizeof(m_modifiers));

	JT_START_BINDING("kitAccessory", KitAccessoryProfile);
	JT_BIND_BYTE(m_eType, "type", true);
	JT_BIND_INT(m_eFilter, "filter", true);
	JT_BIND_OBJECT(m_modifiers, "modifiers", true);
	JT_FINISH_BINDING();
}

bool KitAccessoryProfile::AccessibleToWeapon(const WeaponDef* pWeapon) {
	uint32 all = (uint32)EAccessoryFilter::W_ALL;
	uint32 f = (uint32)m_eFilter;
	uint32 a = (uint32)pWeapon->m_eAvailabilityFilter;
	return (all & f & a);
}