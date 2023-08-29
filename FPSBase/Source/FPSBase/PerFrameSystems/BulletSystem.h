#pragma once
#include "CoreMinimal.h"
#include "PerFrameSystemManager.h"
#include "../PODT/DamageInfo.h"

class FPSBaseCharacter;
class WeaponDef;

class SimpleBullet {
public:
	FVector				m_position;
	FQuat				m_angle;
	int					m_iDamage;
	FPSBaseCharacter*	m_owner;
	WeaponDef*			m_weapon;
	int					m_iDamageTypeOverride;

	SimpleBullet(
		FVector position, 
		FQuat angle, 
		int iDamage, 
		FPSBaseCharacter* owner, 
		WeaponDef* weapon,
		int damageTypeOverride = DMG_BULLET);
};

class SimulatedBullet {
	friend class BulletSystem;
public:
	FVector				m_vPosition;
	FQuat				m_angle;
	int					m_iDamage;
	float				m_flConstantDamageRange;
	float				m_flRelativeDrag;
	float				m_flMuzzleVelocity;
	float				m_flDamageDropoffMultiplier;
	FPSBaseCharacter*	m_owner;
	WeaponDef*			m_weapon;
	bool				m_bPenetrateFlesh;
	int					m_iDamageTypeOverride;

	SimulatedBullet(
		FVector	position, 
		FQuat angle, 
		int iDamage, 
		float flConstantDamageRange, 
		float flRelativeDrag, 
		float flMuzzleVelocity, 
		float flDamageDropoffMultiplier, 
		FPSBaseCharacter* owner, 
		WeaponDef* weapon, 
		bool bPenetrateFlesh,
		int damageTypeOverride = DMG_BULLET);

private:

	//non-configured properties, system controls these
	FVector m_vTrajStart, m_vLastPosition, m_vVelocity;
	float m_flDyingTime;
	bool m_bHasPlayedNearMiss;
	int m_iBounces;
	float m_flHitDropProbability;

	// BG2 - VisualMelon - RayWaitCounter
	int rwc; // this gets set to a max, and when it hits zero, a ray is cast (and it is reset to max (see resetRwc)

	void resetRwc(bool flex);

	bool Think(float dt, bool flex);

	void UpdateVelocity(float dt);

	bool DoTrace(const FVector& step);

	void DoNearMiss() {}

	bool DoHit(FHitResult& tr, float speed);

	void TraceThroughArms(FHitResult* tr);
};

class BulletSystem : PerFrameSystem {
public:
	BulletSystem() : PerFrameSystem(FString("bullets")) {}

	void Init() override;
	void Tick(float deltaSeconds) override;

	bool SpawnSimpleBullet(const SimpleBullet& bullet); //whether or not bullet hits
	void SpawnSimulatedBullet(SimulatedBullet&& bullet);
};


#define BULLET_SIMULATION_FREQUENCY 500 // scaled down from 1000
#define BULLET_SIMULATION_RWC 10 // number of motions between collision detection
