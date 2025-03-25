#include "KitAccessoryProfile.h"
#include "WeaponDefProfile.h"
#include "../BGPlayer.h"

KitAccessoryModifiers g_emptyKitAccessoryModifiers = KitAccessoryModifiers();

KitAdjustmentValues::KitAdjustmentValues() :
	m_iAttack(0),
	m_iDefense(0),
	m_iReach(0),
	m_iStamina(0),
	m_iAccuracy(0),
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
	memset(this, 0, sizeof(KitAccessoryModifiers));

	//set multipliers to 1

	m_flDamageReceivedMultiplier =
		m_flCrouchedSpeedMultiplier =
		m_flAccelerationMultiplier =
		m_flCrouchSpeedMultiplier =
		m_flJumpSpeedMultiplier =
		m_flRecoilMultiplier =
		m_flMeleeStaminaDrainMultiplier =
		m_flMeleeDamageMultiplier =
		m_flJumpMeleeMultiplier = 1.f;

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
	using namespace DataSystem;

	ABGPlayer& p = *pPlayer;

	p.m_pClass = (PlayerClassProfile*)g_classes[m_iClass];

	p.m_pPrimaryWeaponSelection = (WeaponDef*)g_weapons[m_iWeapon1];
	p.m_pSecondaryWeaponSelection = m_iWeapon2 != -1 ? (WeaponDef*)g_weapons[m_iWeapon2] : NULL;
	p.m_pTertiaryWeaponSelection = m_iWeapon3 != -1 ? (WeaponDef*)g_weapons[m_iWeapon3] : NULL;

	p.m_pPrimaryWeaponAccessory1 = (KitAccessoryProfile*)g_accessories[m_iPrimaryAccessory1];
	p.m_pPrimaryWeaponAccessory2 = (KitAccessoryProfile*)g_accessories[m_iPrimaryAccessory2];
	p.m_pSecondaryWeaponAccessory1 = (KitAccessoryProfile*)g_accessories[m_iSecondaryAccessory1];
	p.m_pSecondaryWeaponAccessory2 = (KitAccessoryProfile*)g_accessories[m_iSecondaryAccessory2];

	p.m_pClassAccessory = (KitAccessoryProfile*)g_classAccessories[m_iClassAccessory];

	p.m_pOccupationalPerk = (KitAccessoryProfile*)g_occupationalPerks[m_iOccupationalPerk];
	p.m_pPrimaryPerk = (KitAccessoryProfile*)g_perks[m_iPerk1];
	p.m_pSecondaryPerk = (KitAccessoryProfile*)g_perks[m_iPerk2];
}

void ABGPlayer::AccumulatePerks() {
	typedef KitAccessoryModifiers KAM;

	const KAM* perks[] = {
		&m_pClass->GetProfile()->m_modifiers,
		&m_pPrimaryWeaponAccessory1->m_modifiers,
		&m_pPrimaryWeaponAccessory2->m_modifiers,
		&m_pSecondaryWeaponAccessory1->m_modifiers,
		&g_emptyKitAccessoryModifiers, //&m_pSecondaryWeaponAccessory2->m_modifiers,
		&m_pClassAccessory->m_modifiers,
		&m_pOccupationalPerk->m_modifiers,
		&m_pPrimaryPerk->m_modifiers,
		&m_pSecondaryPerk->m_modifiers
	};

	//before anything else, check whether or not we should exclude m_pSecondaryWeaponAccessory2
	for (int i = 0; i < (sizeof(perks) / sizeof(KAM)); i++) {
		if (perks[i]->HasFlag(EAUF::EXTRA_SECONDARY_ACCESSORY)) {
			perks[4] = &m_pSecondaryWeaponAccessory2->m_modifiers;
			break;
		}
	}

	//zero our sum
	memset(&m_accumulatedPerks, 0, sizeof(m_accumulatedPerks));

	KAM& p = m_accumulatedPerks;

	for (int i = 0; i < (sizeof(perks) / sizeof(KAM)); i++) {
		auto po = perks[i];
		p.m_iUniqueFlags |= po->m_iUniqueFlags;

		//general and movement stuff
		p.m_flWeight += po->m_flWeight;
		p.m_flMaxWeightModifier += po->m_flMaxWeightModifier;
		p.m_flWeightReductionHolstered += po->m_flWeightReductionHolstered;
		p.m_flDamageReceivedMultiplier += po->m_flDamageReceivedMultiplier;
		p.m_flDrawSpeedModifier += po->m_flDrawSpeedModifier;
		p.m_flCrouchedSpeedMultiplier += po->m_flCrouchedSpeedMultiplier;
		p.m_flAccelerationMultiplier += po->m_flAccelerationMultiplier;
		p.m_flJumpSpeedMultiplier += po->m_flJumpSpeedMultiplier;
		p.m_flCrouchSpeedMultiplier += po->m_flCrouchSpeedMultiplier;
		p.m_flPlayerSizeMultiplier += po->m_flPlayerSizeMultiplier;

		//shooting stuff
		p.m_flLockTimeModifier += po->m_flLockTimeModifier;
		p.m_flIgniteTimeModifier += po->m_flIgniteTimeModifier;
		p.m_flRecoilMultiplier += po->m_flRecoilMultiplier;
		p.m_flReloadMovementSpeedModifier += po->m_flReloadMovementSpeedModifier;
		p.m_flReloadSpeedModifier += po->m_flReloadSpeedModifier;

		//melee stuff
		p.m_flMeleeIntervalModifier += po->m_flMeleeIntervalModifier;
		p.m_flMeleeHitIntervalModifier += po->m_flMeleeHitIntervalModifier;
		p.m_flMeleeDamageMultiplier += po->m_flMeleeDamageMultiplier;
		p.m_flMeleeStaminaDrainMultiplier += po->m_flMeleeStaminaDrainMultiplier;
		p.m_flJumpMeleeMultiplier += po->m_flJumpMeleeMultiplier;

		//other
		p.m_flFuseLengthModifier += po->m_flFuseLengthModifier;
		p.m_flGrenadeSpeedModifier += po->m_flGrenadeSpeedModifier;

		p.m_iAVsCapModifier += po->m_iAVsCapModifier;
		p.m_iAVsAmountModifier += po->m_iAVsAmountModifier;
	}

	//add weights from weapons and ammo
	p.m_flWeight += m_pPrimaryWeaponSelection->m_flWeight;
	if (m_pSecondaryWeaponSelection) p.m_flWeight += m_pSecondaryWeaponSelection->m_flWeight;
	if (m_pTertiaryWeaponSelection) p.m_flWeight += m_pTertiaryWeaponSelection->m_flWeight;

	//TODO include ammo
}

void ABGPlayer::AccumulateAVs() {
	typedef KitAdjustmentValues KAV;
	
	const KAV* adjustments[] = {
		&m_pClass->GetProfile()->m_adjustments,
		&m_pClassAccessory->m_adjustments,
		&m_pOccupationalPerk->m_adjustments,
		&m_pPrimaryPerk->m_adjustments,
		&m_pSecondaryPerk->m_adjustments
	};

	//zero our sum
	memset(&m_accumulatedAVs, 0, sizeof(m_accumulatedAVs));

	KAV& a = m_accumulatedAVs;

	for (int i = 0; i < (sizeof(adjustments) / sizeof(KAV)); i++) {
		auto ao = adjustments[i];
		a.m_iAttack += ao->m_iAttack;
		a.m_iDefense += ao->m_iDefense;
		a.m_iReach += ao->m_iReach;
		a.m_iStamina += ao->m_iStamina;
		a.m_iAccuracy += ao->m_iAccuracy;
		a.m_iSpeed += ao->m_iSpeed;
		a.m_iReload += ao->m_iReload;
	}
}

