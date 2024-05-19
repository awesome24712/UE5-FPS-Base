#pragma once
#include "CoreMinimal.h"
#include "../Util.h"

class WeaponDef;

//Represents anything chosen in the classmenu 
//which isn't a weapon manually swapped too
// ex. includes bayonets, but not muskets
// ex. includes perks, but not cosmetics
// Controls which classmenu slot the accessory goes into
enum class EAccessoryType : uint8 {
	CLASS_PERK = 0, //
	CLASS_ACCESSORY, //flash hole pick, butterknife, backpack, bandage
	WEAPON_ATTACHMENT, //Bayonet, lock adjustment, trigger
};

// Fine-grain filtering on which accessories are available based on
// other choices. Ex. you can't put a bayonet onto a sword
enum class EAccessoryFilter : uint32 {
	//class-specific perks
	C_INFANTRY			= 1,
	C_OFFICER			= 1 << 1,
	C_SNIPER			= 1 << 2,
	C_SLASHER			= 1 << 3,
	C_LIGHT_INFANTRY	= 1 << 4,
	C_GRENADIER			= 1 << 5,
	C_ALL				= (1 << 6) - 1,


	//weapon-specific filters
	W_MUSKET			= 1 << 6, //standard infantry musket: bayonet, sling
	//W_CARBINE			= 1 << 7,
	W_RIFLE				= 1 << 8, //snipers: lock, trigger improvements
	W_SWORD				= 1 << 9, //weighted grip
	W_KNIFE				= 1 << 10, //poison blade, serrated edge
	W_GRENADE			= 1 << 11, //fuse, explosive type adjustments
	W_BLUNDERBUSS		= 1 << 12, //sling, 
	W_POLEARM			= 1 << 13, //speartip, shaft length
	W_ALL				= (1 << 14) - 1,
};

//For data-independent on/off modifiers
enum class EAccessoryUniqueFlags {
	WATER_RESISTANT = BF(0),
	SERRATED_EDGE = BF(1),
	POISONED_EDGE = BF(2),
	WATER_RESISTANT_FUSE = BF(3),
	ATTACK_DURING_SPRINT = BF(4),
	MELEE_UNDERWATER = BF(5)
};

//Each Kit Accessory profile has their own values for these
//Each player has one of these too, acummulating all modifiers so we don't have to do as much math each frame
struct KitAccessoryModifiers : public IJsonBindable {
	//general
	uint32	m_iUniqueFlags; //For data-independent on/off modifiers

	float	m_flWeight;
	float	m_flSpeedModifier;
	float	m_flMaxWeightModifier;
	float	m_flWeightReductionHolstered;
	float	m_flStaminaDrainMultiplier;
	float	m_flDamageReceivedMultiplier;
	float	m_flLowDamageReceivedMultiplier;

	//shooting
	float	m_flLockTimeModifier;
	float	m_flAccuracyModifier;
	float	m_flRecoilModifier;
	float	m_flReloadMovementSpeedModifier;
	float	m_flReloadSpeedModifier;

	//melee
	float	m_flMeleeIntervalModifier;
	int8	m_iMeleeRangeExtension;

	//other
	float	m_flFuseLengthModifier;
	float	m_flGrenadeSpeedModifier;

	KitAccessoryModifiers();
};

enum class EAccessoryCosmeticFlags : uint32 {

};

class KitAccessoryProfile : public IJsonBindable {
public:
	EAccessoryType			m_eType;
	bf						m_eFilter; //bitfield of filters

	KitAccessoryModifiers	m_modifiers;
	EAccessoryCosmeticFlags m_cosmetics;

	KitAccessoryProfile();

	inline bool AccessibleToAllClasses() const {
		uint32 f = (uint32)m_eFilter;
		uint32 all = (uint32)EAccessoryFilter::C_ALL;
		return (f & all) == all
			|| (f & all) == 0;
	}

	inline bool AccessibleToClass(EClassNumber iClass) const {
		return AccessibleToAllClasses() || ((uint32)m_eFilter & BF((uint32)iClass));
	}

	bool AccessibleToWeapon(const WeaponDef* pWeapon);
};

