#pragma once
#include "CoreMinimal.h"
#include "../Util.h"

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


	//weapon-specific filters
	W_MUSKET			= 1 << 6, //standard infantry musket: bayonet, sling
	//W_CARBINE			= 1 << 7,
	W_RIFLE				= 1 << 8, //snipers: lock, trigger improvements
	W_SWORD				= 1 << 9, //weighted grip
	W_KNIFE				= 1 << 10, //poison blade, serrated edge
	W_GRENADE			= 1 << 11, //fuse, explosive type adjustments
	W_BLUNDERBUSS		= 1 << 12, //sling, 
	W_POLEARM			= 1 << 13, //speartip, shaft length
};

class PlayerClassAccessoryProfile : public IJsonBindable {
public:
	EAccessoryType		m_eType;
	bf					m_eFilter; //bitfield of filters

	//int					m_iUniqueId;
	float				m_flWeight;
	float				m_flSpeedModifier;
	float				m_flMaxWeightModifier;

	PlayerClassAccessoryProfile();
};