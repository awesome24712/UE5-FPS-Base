#pragma once
#include "../Util.h"

class Faction;
class PlayerClassProfile;
class WeaponDef;
class KitAccessoryProfile;
class AmmoProfile;
class SoundscapeProfile;

namespace DataSystem {
	extern TArray<Faction*> g_factions;
	extern TArray<PlayerClassProfile*> g_classes;
	extern TArray<WeaponDef*> g_weapons;
	extern TArray<KitAccessoryProfile*> g_accessories;
	extern TArray<KitAccessoryProfile*> g_classAccessories;
	extern TArray<KitAccessoryProfile*> g_occupationalPerks;
	extern TArray<KitAccessoryProfile*> g_perks;
	extern TArray<AmmoProfile*> g_ammoTypes;


	void InitDataSystem(); //loads all of the above data
};