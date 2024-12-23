#include "KitAccessoryProfile.h"
#include "WeaponDefProfile.h"
#include "../BGPlayer.h"

KitAdjustmentValues::KitAdjustmentValues() :
	m_iAttack(0),
	m_iDefense(0),
	m_iReach(0),
	m_iStamina(0),
	m_iSpeed(0),
	m_iReload(0)
{
	JT_START_BINDING("kitAdjustmentValues", KitAdjustmentValues);
	JT_BIND_BYTE(m_iAttack, "attack", false);
	JT_BIND_BYTE(m_iDefense, "defense", false);
	JT_BIND_BYTE(m_iReach, "reach", false);
	JT_BIND_BYTE(m_iStamina, "stamina", false);
	JT_BIND_BYTE(m_iAccuracy, "accuracy", false);
	JT_BIND_BYTE(m_iSpeed, "speed", false);
	JT_BIND_BYTE(m_iReload, "reload", false);
	JT_FINISH_BINDING();
}


KitAccessoryModifiers::KitAccessoryModifiers() {
	//general
	JT_START_BINDING("kitAccessoryModifiers", KitAccessoryModifiers);
	JT_BIND_INT(m_iUniqueFlags, "uniqueFlags", false);

	JT_BIND_FLOAT(m_flWeight, "weight", false);
	//JT_BIND_FLOAT(m_flSpeedModifier, "speedModifier", false);
	JT_BIND_FLOAT(m_flMaxWeightModifier, "maxWeightModifier", false);
	JT_BIND_FLOAT(m_flWeightReductionHolstered, "weightReductionHolstered", false);
	JT_BIND_FLOAT(m_flDamageReceivedMultiplier, "damageReceivedMultiplier", false);
	JT_BIND_FLOAT(m_flDrawSpeedModifier, "drawSpeedModifier", false);
	JT_BIND_FLOAT(m_flCrouchedSpeedMultiplier, "crouchedSpeedMultiplier", false);
	JT_BIND_FLOAT(m_flAccelerationMultiplier, "accelerationMultiplier", false);
	JT_BIND_FLOAT(m_flCrouchSpeedMultiplier, "crouchSpeedMultiplier", false);
	JT_BIND_FLOAT(m_flJumpSpeedMultiplier, "jumpSpeedMultiplier", false);
	//JT_BIND_FLOAT(m_flLowDamageReceivedMultiplier, "lowDamageReceivedMultiplier", false);

	//shooting
	JT_BIND_FLOAT(m_flLockTimeModifier, "lockTimeModifier", false);
	JT_BIND_FLOAT(m_flIgniteTimeModifier, "igniteTimeModifier", false);
	//JT_BIND_FLOAT(m_flAccuracyMultiplier, "accuracyModifier", false);
	JT_BIND_FLOAT(m_flRecoilMultiplier, "recoilMultiplier", false);
	JT_BIND_FLOAT(m_flReloadMovementSpeedModifier, "reloadMovementSpeedModifier", false);
	JT_BIND_FLOAT(m_flReloadSpeedModifier, "reloadSpeedModifier", false);

	//melee
	JT_BIND_FLOAT(m_flMeleeIntervalModifier, "meleeIntervalModifier", false);
	JT_BIND_FLOAT(m_flMeleeHitIntervalModifier, "meleeHitIntervalModifier", false);
	JT_BIND_FLOAT(m_flMeleeStaminaDrainMultiplier, "meleeStaminaDrainMultiplier", false);
	JT_BIND_FLOAT(m_flMeleeDamageMultiplier, "meleeDamageMultiplier", false);
	JT_BIND_FLOAT(m_flJumpMeleeMultiplier, "jumpMeleeMultiplier", false);

	//other
	JT_BIND_FLOAT(m_flFuseLengthModifier, "fuseLengthModifier", false);
	JT_BIND_FLOAT(m_flGrenadeSpeedModifier, "grenadeSpeedModifier", false);

	JT_BIND_BYTE(m_iAVsCapModifier, "AVsCapModifier", false);
	JT_BIND_BYTE(m_iAVsAmountModifier, "AVsAmountModifier", false);

	JT_FINISH_BINDING();
}

KitAccessoryProfile::KitAccessoryProfile() {
	m_eType = EAccessoryType::PERK;
	m_eFilter = 0;

	memset(&m_modifiers, 0, sizeof(m_modifiers));

	JT_START_BINDING("kitAccessory", KitAccessoryProfile);
	JT_BIND_BYTE(m_eType, "type", true);
	JT_BIND_INT(m_eFilter, "filter", true);
	JT_BIND_INT(m_cosmetics, "cosmetics", false);
	JT_BIND_OBJECT(m_modifiers, "modifiers", false);
	JT_BIND_OBJECT(m_adjustments, "adjustments", false);
	JT_BIND_STRING(m_label, "label", false);
	JT_BIND_STRING(m_desc, "desc", false);
	JT_BIND_STRING(m_warning, "warning", false);
	JT_BIND_BOOLEAN(m_bTwoSlots, "twoSlots", false);
	JT_FINISH_BINDING();
}

bool KitAccessoryProfile::AccessibleToWeapon(const WeaponDef* pWeapon) {
	uint32 all = (uint32)EAccessoryFilter::W_ALL;
	uint32 f = (uint32)m_eFilter;
	uint32 a = (uint32)pWeapon->m_eAvailabilityFilter;
	return !f || (all & f & a);
}

void FKitLoadout::LoadOntoPlayer(ABGPlayer* pPlayer) {

	const TArray<IJsonBindable*>& classes = JTClassBindingSet::FindBindingSet("playerclass")->GetAll();
	const TArray<IJsonBindable*>& weapons = JTClassBindingSet::FindBindingSet("WeaponDef")->GetAll();
	const TArray<IJsonBindable*>& accessories = JTClassBindingSet::FindBindingSet("kitAccessory")->GetAll();

	ABGPlayer& p = *pPlayer;

	p.m_pClass = (PlayerClass*)classes[m_iClass];

	p.m_pPrimaryWeaponSelection = (WeaponDef*)weapons[m_iWeapon1];
	p.m_pSecondaryWeaponSelection = m_iWeapon2 != -1 ? (WeaponDef*)weapons[m_iWeapon2] : NULL;
	p.m_pTertiaryWeaponSelection = m_iWeapon3 != -1 ? (WeaponDef*)weapons[m_iWeapon3] : NULL;

	p.m_pPrimaryWeaponPerk = (KitAccessoryProfile*)accessories[m_iWeaponPerk1];
	p.m_pSecondaryWeaponPerk = (KitAccessoryProfile*)accessories[m_iWeaponPerk2];

	p.m_pClassSpecificPerk = (KitAccessoryProfile*)accessories[m_iPerk1];
	p.m_pPrimaryClassPerk = (KitAccessoryProfile*)accessories[m_iPerk2];
	p.m_pSecondaryClassPerk = (KitAccessoryProfile*)accessories[m_iPerk3];
}

