#pragma once
#include "CoreMinimal.h"
#include "../Util.h"

struct FDamageInfo;

//per-game stats, also tracked locally and persistently
class PlayerStats {
public:
	uint64 m_iScore;
	uint64 m_iKills;
	uint64 m_iDeaths;
	uint64 m_iDamage;
	
	uint64 m_iShotsFired;
	uint64 m_iShotsHit;
	uint64 m_iStabsFired;
	uint64 m_iStabsHit;

	inline double GetPercentageShotsHit() const {
		return ((double) m_iShotsHit) / ((double)m_iShotsFired);
	}

	inline double GetPercentageMeleeHit() const {
		return ((double)m_iStabsHit) / ((double)m_iStabsFired);
	}
};

class PerWeaponStats {
public:
	FName m_weaponCodeName;
	PlayerStats m_stats;
};

class PlayerStatsWrapper {
public:
	PlayerStats				m_currentGameStats;
	PlayerStats				m_lifetimeStats;
	TArray<PerWeaponStats>	m_lifetimeWeaponStats;

	static void ProcessDamageEvent(const FDamageInfo& info, PlayerStatsWrapper* pAttacker, PlayerStatsWrapper* pDefender);

	static void ReadPlayerLifetimeStatsFromDisk();
	static void WritePlayerLifetimeStatsToDisk();
	
	static void ReadServerLifetimeStatsFromDisk();
	static void WriteServerLifetimeStatsToDisk();
};