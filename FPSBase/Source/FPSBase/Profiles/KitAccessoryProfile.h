#pragma once
#include "CoreMinimal.h"
#include "../Util.h"
#include "KitAccessoryProfile.generated.h"

class WeaponDef;
class ABGPlayer;

//Represents anything chosen in the classmenu 
//which isn't a weapon manually swapped too
// ex. includes bayonets, but not muskets
// ex. includes perks, but not cosmetics
// Controls which classmenu slot the accessory goes into
enum class EAccessoryType : uint8 {
	WEAPON_ATTACHMENT = 1, //Bayonet, lock adjustment, trigger
	CLASS_ACCESSORY, //flash hole pick, butterknife, backpack, bandage
	OCCUPATIONAL_PERK,
	PERK, //
	
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
	W_CARBINE			= 1 << 7, //cavalry style sling
	W_RIFLE				= 1 << 8, //snipers: lock, trigger improvements
	W_SWORD				= 1 << 9, //weighted grip
	W_KNIFE				= 1 << 10, //poison blade, serrated edge
	W_AXE				= 1 << 11, //axes and clubs
	W_GRENADE			= 1 << 12, //fuse, explosive type adjustments
	W_BLUNDERBUSS		= 1 << 13, //sling, 
	W_POLEARM			= 1 << 14, //speartip, shaft length
	W_ALL				= (1 << 15) - 1,
};

//For data-independent on/off modifiers
enum class EAccessoryUniqueFlags {
	WATER_RESISTANT = BF(0),
	SERRATED_EDGE = BF(1),
	POISONED_EDGE = BF(2),
	WATER_RESISTANT_FUSE = BF(3),
	ATTACK_DURING_SPRINT = BF(4),
	MELEE_UNDERWATER = BF(5),
	DAMAGE_NOT_EFFECTS = BF(6), //no stamina drain from hit
	BETTER_HEALING = BF(7),
	THROW_POLARMS = BF(8),
	LOCKSMITH = BF(9),
	FAST_NATURE_MOVEMENT = BF(10),
	ATTACK_AVS_ON_BULLETS = BF(11),
	HEADHUNTER = BF(12),
	FLAG_BEARER = BF(13),
	SPAWN_DRUNK = BF(14),
	OFFICER_BUFFS = BF(15),
	AUTOSTAB = BF(16),
	RELOAD_CANCEL = BF(17),
	OFFICER_BUFFS_EVERYWHERE = BF(18),
	EXTRA_SECONDARY_ACCESSORY = BF(19),
	ACCURATE_JUMPING = BF(20),
	BULLETS_FATAL_ON_OFFICERS = BF(21),
	BACKSTABBER = BF(22),
	DOWNSWINGER = BF(23),
	DONT_DROP_GRENADES = BF(24)
};
typedef EAccessoryUniqueFlags EAUF;

struct KitAdjustmentValues : public IJsonBindable {
	KitAdjustmentValues();

	uint8
		m_iAttack, //melee damage output modifier, 1 unit = 1 damage
		m_iDefense, //damage recieved modifier, 1 unit = 1 damage
		m_iReach, //melee range modifier, 1 unit = ?
		m_iStamina, //max stamina quantity modifier, 1 unit = 1 stamina
		m_iAccuracy, //accuracy modifier, 1 unit = ?
		m_iSpeed, //speed modifier, 1 unit = 1%
		m_iReload; //reload speed modifier, 1 unit = 0.3s ?

	inline int Total() const {
		return m_iAttack + m_iDefense + m_iReach + m_iStamina + m_iAccuracy + m_iSpeed + m_iReload;
	}
};

//Each Kit Accessory profile has their own values for these
//Each player has one of these too, acummulating all modifiers so we don't have to do as much math each frame
struct KitAccessoryModifiers : public IJsonBindable {
	//general
	uint32	m_iUniqueFlags; //For data-independent on/off modifiers

	float	m_flWeight;
	//float	m_flSpeedModifier;
	float	m_flMaxWeightModifier;
	float	m_flWeightReductionHolstered;
	float	m_flDamageReceivedMultiplier;
	float	m_flDrawSpeedModifier;
	float	m_flCrouchedSpeedMultiplier;
	float	m_flAccelerationMultiplier;
	float	m_flJumpSpeedMultiplier;
	float	m_flCrouchSpeedMultiplier;
	float	m_flPlayerSizeMultiplier;


	//shooting
	float	m_flLockTimeModifier;
	float	m_flIgniteTimeModifier;
	//float	m_flAccuracyMultiplier;
	float	m_flRecoilMultiplier;
	float	m_flReloadMovementSpeedModifier;
	float	m_flReloadSpeedModifier;

	//melee
	float	m_flMeleeIntervalModifier;
	float	m_flMeleeHitIntervalModifier;
	//int8	m_iMeleeRangeExtension;
	float	m_flMeleeDamageMultiplier;
	float	m_flMeleeStaminaDrainMultiplier;
	float	m_flJumpMeleeMultiplier;

	//other
	float	m_flFuseLengthModifier;
	float	m_flGrenadeSpeedModifier;



	int8	m_iAVsCapModifier;
	int8	m_iAVsAmountModifier;

	KitAccessoryModifiers();
};

enum class EAccessoryCosmeticFlags : uint32 {

};

class KitAccessoryProfile : public IJsonBindable {
public:
	EAccessoryType			m_eType;
	bf						m_eFilter; //bitfield of filters
	FString					m_factionLimitation;

	KitAccessoryModifiers	m_modifiers;
	EAccessoryCosmeticFlags m_cosmetics;
	KitAdjustmentValues		m_adjustments;
	bool					m_bTwoSlots; //for perks, if true both spots are occupied


	FString					m_label;
	FString					m_desc;
	FString					m_warning;


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

//--------------------------------------------------------------------
// This struct represents loadout selection in a networkable way
//--------------------------------------------------------------------
USTRUCT()
struct FKitLoadout {
	GENERATED_BODY()

	//indexes into factory lists, i.e. the arrays returned by JTClassBindingSet::GetAll()
	//these indexes can change as profiles are added to the game
	uint8 m_iClass;

	uint8 m_iWeapon1;
	uint8 m_iWeapon2; //-1 for none
	uint8 m_iWeapon3;

	uint8 m_iPrimaryAccessory1;
	uint8 m_iPrimaryAccessory2;

	uint8 m_iSecondaryAccessory1;
	uint8 m_iSecondaryAccessory2;

	uint8 m_iClassAccessory;

	uint8 m_iPerk1;
	uint8 m_iPerk2;
	uint8 m_iPerk3;

	//Updates variables inside pPlayer to match these indexes
	void LoadOntoPlayer(ABGPlayer* pPlayer);
};
