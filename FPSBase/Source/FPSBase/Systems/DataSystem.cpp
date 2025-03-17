#include "DataSystem.h"
#include "../CommonHeaders.h"


namespace DataSystem {
	TArray<Faction*> g_factions;
	TArray<PlayerClassProfile*> g_classes;
	TArray<WeaponDef*> g_weapons;
	TArray<KitAccessoryProfile*> g_accessories;
	TArray<KitAccessoryProfile*> g_classAccessories;
	TArray<KitAccessoryProfile*> g_occupationalPerks;
	TArray<KitAccessoryProfile*> g_perks;
	TArray<AmmoProfile*> g_ammoTypes;

	void InitDataSystem() {
		NTreeManager::LoadBindablesFromFolder(g_factions, "Factions");
		NTreeManager::LoadBindablesFromFolder(g_classes, "Classes");
		NTreeManager::LoadBindablesFromFolder(g_weapons, "Weapons");
		NTreeManager::LoadBindablesFromFile(g_accessories, "Accessories.json");
		NTreeManager::LoadBindablesFromFile(g_classAccessories, "ClassAccessories.json");
		NTreeManager::LoadBindablesFromFile(g_occupationalPerks, "OccupationalPerks.json");
		NTreeManager::LoadBindablesFromFile(g_perks, "Perks.json");
		NTreeManager::LoadBindablesFromFile(g_ammoTypes, "AmmoTypes.json");
	}
};