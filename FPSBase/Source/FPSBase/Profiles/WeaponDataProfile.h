#pragma once
#include "CoreMinimal.h"
#include "../Util.h"
#define DAMAGE_OVER_RANGE_GRAPH_RANGE_MAX 401

enum {
	ATTACKTYPE_NONE = 0,
	ATTACKTYPE_STAB,
	ATTACKTYPE_SLASH,
	ATTACKTYPE_FIREARM,
	/*
	other attacktypes might be
	ATTACKTYPE_THROW,

	*/
};


//This was ported from BG2
//-----------------------------------------------------------------------------
// Purpose: Outlines stats for a left or right mouse button attack.
//-----------------------------------------------------------------------------
struct WeaponAttackInfo : public IJsonBindable {
	int			m_iAttacktype;
	//Activity	m_iAttackActivity,
		//m_iAttackActivityEmpty;
	bool 	m_bFiresUnderwater;

	float	m_flRange; //used as max range for melee and for non simulated bullets
	int		m_iDamage;
	float	m_flRecoil,	
			// - melee only
			m_flCosAngleTolerance,	//tolerance of melee hits(sqrt(0.5) for crowbar, or 45 degrees)
			m_flRetraceDuration,
			m_flRetraceDelay,
			m_flAttackrate;
	int		m_iStaminaDrain;			//stamina drained by attack

	//cone values.. firearms only
	float	m_flStandMoving,
			m_flStandStill,
			m_flCrouchMoving,
			m_flCrouchStill,
			// For iron sights. -HairyPotter
			m_flStandAimStill,
			m_flCrouchAimStill,
			//
			m_flConstantDamageRange,	//how long until we start losing damage?
			m_flRelativeDrag;			//how does the drag on this bullet compare to a musket's?

	inline bool HasMelee() const;

	WeaponAttackInfo();
};

bool WeaponAttackInfo::HasMelee() const {
	return m_iAttacktype == ATTACKTYPE_SLASH || m_iAttacktype == ATTACKTYPE_STAB;
}


//-----------------------------------------------------------------------------
// Purpose: Stores all weapon stats into a single definition, rather than including
//		copies in every instance of a weapon.
//-----------------------------------------------------------------------------
class WeaponDef : public IJsonBindable {
public:
	//Two attackinfos for the two modes of attack
	WeaponAttackInfo	m_Attackinfos[2];
	const char*			m_pBayonetDeathNotice;

	//values related to internal ballistics
	float	m_flShotAimModifier;//applies to aim cone. afterwards m_flShotSpread applies. should be negative
	float	m_flShotSpread;		//used when firing shot, if the current gun is capable of that
	uint8	m_iNumShot;
	int		m_iDamagePerShot;
	bool	m_bShotOnly;
	float	m_flMuzzleVelocity;
	float	m_flShotMuzzleVelocity;	//muzzle velocity when firing buckshot
	float	m_flDamageDropoffMultiplier;
	bool	m_bPenetrateFlesh;
	float	m_flVerticalAccuracyScale;
	float	m_flZeroRange;		//range to zero the gun in at
	

	uint8	m_iOwnerSpeedModOnKill; //BG3 - was for Native war club, removed for now
	uint8	m_iAerialDamageMod; //damage modifier for hits in the air
	int		m_iExtraDamageTypes; //ex. DMG_BURN from burning man weapon

	bool	m_bDontAutoreload;
	bool	m_bCantAbortReload;
	float	m_flReloadMovementSpeedModifier;

	//WeaponType m_eWeaponType;

	float	m_flIronsightFOVOffset;
	bool	m_bWeaponHasSights;
	bool	m_bDemoteNonHeadhitsToSecondaryDamage;
	//bool	m_bQuickdraw;
	//bool	m_bSlowDraw;
	float	m_flIronsightsTime;
	bool	m_bBreakable;

	float	m_fHolsterTime;
	float	m_flLockTime;
	float	m_flRandomAdditionalLockTimeMax;
	//float	m_flApproximateReloadTime; //approximate because the actual time used is based on the animation. These values need to be kept up-to-date with animation times


	FString m_weaponName;
	//const char* m_pszWeaponDefName;
	WeaponDef();

	static const WeaponDef* GetDefForWeapon(const FString& pszWeaponName);
	static const WeaponDef* GetDefault(); //for non-standard weapons, to avoid crashing
	inline bool HasMelee() const { return m_Attackinfos[0].HasMelee() || m_Attackinfos[1].HasMelee(); }

#ifdef CLIENT_DLL
	friend class CDamageOverRangeMenu;
	FColor GetGraphColor() const { return m_graphColor; }

private:
	FColor	m_graphColor;
	mutable int16 m_aSimulatedDamageData[DAMAGE_OVER_RANGE_GRAPH_RANGE_MAX];
	mutable float m_aSimulatedDropData[DAMAGE_OVER_RANGE_GRAPH_RANGE_MAX];
	mutable bool m_bDataSimulated = false;
#endif
};

//Don't use this macro by itself - combine it with DEC_BG3_WEAPON_ENT, unless you know what you're doing
//This forward declares a weapon definition of the format g_Def_##weaponName
/*#define DEC_BG3_WEAPON_DEF(weaponName) \
	class WeaponDef_##weaponName : public WeaponDef { public: WeaponDef_##weaponName(); }; \
	const WeaponDef_##weaponName g_Def_##weaponName;

//Use this to define constructor for weapon def
#define DEF_BG3_WEAPON_DEF(weaponName) \
	WeaponDef_##weaponName::WeaponDef_##weaponName() : WeaponDef(#weaponName)*/