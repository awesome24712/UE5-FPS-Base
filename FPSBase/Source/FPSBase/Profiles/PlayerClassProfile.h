#pragma once
#include "../Util.h"
#include "WeaponDefProfile.h"

/*
PLAYER-MODELS LIST
This list is used to precache and validate the player models.
A forward declaration here exposes it to rest of program
*/
extern const char* g_ppszBritishPlayerModels[];
extern const char* g_ppszAmericanPlayerModels[];

extern ConVar mp_year_accuracy;
extern ConVar mp_year;

class FPSBaseCharacter;

//forward declaration
namespace NClassQuota {
	struct SPopulationCounter;
}

class GunKit : public IJsonBindable {
	friend class PlayerClass;
	friend class CKitButton;
	friend class CKitSelectionButton;
public:
	GunKit();

	//FName m_weaponPrimaryName;
	//FName m_weaponSecondaryName;
	//FName m_weaponTertiaryName;


	bool	m_bAllowBuckshot = false;
	uint8	m_iAmmoOverrideCount = 0;
	int8	m_iMovementSpeedModifier = 0;
	int8	m_iSleeveTextureOverride = -1;
	//FString m_ammoOverrideName = "";
	//char* m_pszPlayerModelOverrideName = nullptr;

	uint16	m_iMinYear = 0;
	uint16	m_iMaxYear = 1860;
	bool	m_bDisabledOnHistoricityDisabled = false;
	//UnlockableBit m_iControllingBit = (UnlockableBit)0;

	//inline bool ProgressionLocked() const { return m_iControllingBit != 0; }

//#ifdef CLIENT_DLL
	FName GetLocalizedName() const { return m_pLocalizedName; }
	FText GetLocalizedDesc() const { return m_pLocalizedDesc; }

	bool	AvailableForCurrentYear() const;

public:
	void	SetLocalizedName(const FName& token) { m_pszLocalizedNameOverride = token; }
	void	SetLocalizedDesc(const FText& token) { m_pszLocalizedDescOverride = token; }

private:
	mutable FName m_pszLocalizedNameOverride = "";
	mutable FText m_pszLocalizedDescOverride = FText::GetEmpty();
	mutable FName m_pLocalizedName;
	mutable FText m_pLocalizedDesc;
//#endif
};

//#define TOTAL_BRIT_CLASSES 6 //same as above
//#define TOTAL_AMER_CLASSES 5 // merged state militia and minuteman classes
//#define TOTAL_NUM_CLASSES (TOTAL_BRIT_CLASSES + TOTAL_AMER_CLASSES) //used to build the list of classes and model lists

/*
A "class" in BG3 is unique with its own model, skins, weapons, and stats.
I'm tired of these stats and information being spread out into unwieldly functions,
so I'm putting them all into these separate files
*/
class PlayerClass : public IJsonBindable {

public:
	const char* m_pszAbbreviation; //for example, "off" or "gre", used for player class limits
protected:
	static void postClassConstruct(PlayerClass*);

public:

	PlayerClass();

	//uint8			m_iDefaultTeam;
	uint8			m_iRoleNumber = 0; //for backwards-compatibility

	uint8			m_iDefaultPrimaryAmmoCount;
	uint8			m_iDefaultSecondaryAmmoCount = 0;
	//const char*		m_pszPrimaryAmmo = "Musket";
	//const char*		m_pszSecondaryAmmo = "Grenade";


	mutable float	m_flBaseSpeedCalculated = 190.f; //these are for server-wide speed modifications
	float			m_flBaseSpeedOriginal = 190.f;
	float			m_flFlagWeightMultiplier = 1.0f; //multiplier for speed while carrying flag

	//const char*		m_pszPlayerModel;
	//const char*		m_pszJoinName = nullptr;

//#define				NUM_POSSIBLE_WEAPON_KITS 9
	TArray<GunKit*>	m_aWeapons;
private:
	mutable uint8	m_iChooseableKits;
public:

	//uint8			m_iSkinDepth = 1; //how many skin variations per uniform
protected:
	uint8			m_iNumUniforms = 1; //how many uniforms?
public:
	//uint8			m_iSleeveBase = 0; //chosen sleeve skin is m_iSleeveBase + pOwner->m_iClassSkin - 1
	//uint8			m_iArmModel = 0;
	//int8			m_iSleeveInnerModel = 0;

	//bool			m_bLastUniformRestricted = false; //beta tester only uniforms
	//const char*		m_pszDroppedHat = 0;
//#define				NUM_POSSIBLE_UNIFORMS 4
//	const char*		m_pszUniformModelOverrides[NUM_POSSIBLE_UNIFORMS]; //per-uniform model overrides
//	int8			m_aUniformSleeveOverrides[NUM_POSSIBLE_UNIFORMS]; //per-uniform sleeve base overrides
//	int8			m_aUniformArmModelOverrides[NUM_POSSIBLE_UNIFORMS]; //per-uniform sleeve model overrides

	//progression system uniform stuff
	bool			m_bDefaultRandomUniform = false;
	//UnlockableBit	m_iDerandomizerControllingBit = (UnlockableBit)0; //if unlockable profile has this bit, let us choose
	//UnlockableBit	m_aUniformControllingBits[NUM_POSSIBLE_UNIFORMS]; //tells whether or not each uniform is locked
	//bool			m_aUniformControllingBitsActive[NUM_POSSIBLE_UNIFORMS]; //whether or not use the locking uniform bits

	bool			m_bCanDoVcommBuffs = false; //this will be true for officer
	bool			m_bHasImplicitDamageResistance = false;
	bool			m_bHasImplicitDamageWeakness = false;
	bool			m_bNerfResistance = false;

	//Functions
	//inline bool		isAmerican() const { return m_iDefaultTeam == TEAM_AMERICANS; }
	//inline bool		isBritish() const { return m_iDefaultTeam == TEAM_BRITISH; }

	inline uint8		numChooseableWeapons() const { return m_iChooseableKits; }
	//uint8				numChooseableUniformsForPlayer(FPSBaseCharacter* pPlayer) const; // { return m_bForceRandomUniform ? 1 : m_iNumUniforms; }
	inline uint8		numUniforms() const { return m_iNumUniforms; }
	const WeaponDef*	getWeaponDef(uint8 iKit) const;
	void				getWeaponDef(uint8 iKit, const WeaponDef** ppPrimary, const WeaponDef** ppSecondary, const WeaponDef** ppTertiary) const;
	const GunKit*		getWeaponKitChooseable(uint8 iWeapon) const; //indexes choosable weapons, skipping over non-choosable ones.



#ifdef CLIENT_DLL
	bool		shouldHaveWeaponSelectionMenu() const;

	//Might as well store the menu-related items here instead of making separate data structures.
	//With this, each class will remember its last selected gun, ammo, and uniform kits.
private:
	mutable ConVar* m_pcvWeapon = nullptr;
	mutable ConVar* m_pcvAmmo = nullptr;
	mutable ConVar* m_pcvUniform = nullptr;
public:
	//It's a bit wierd to see these as const functions, but it's okay because they aren't actually changing the stats of the class.
	void SetDefaultWeapon(uint8 iWeapon)	const; //0-based
	void SetDefaultAmmo(uint8 iAmmo)		const; //0-based
	void SetDefaultUniform(uint8 iUniform) const; //0-based
	uint8 GetDefaultWeapon(void)			const { return m_pcvWeapon->GetInt(); }
	uint8 GetDefaultAmmo(void)				const { return m_pcvAmmo->GetInt(); }
	uint8 GetDefaultUniform(void)			const { return m_pcvUniform->GetInt(); }

	//This function links all classes to external data stored in ConVars and localizer
	//multi-call safe; the function will do nothing after the first call
	static void InitClientRunTimeData(void);
private:
	static void InitPrevKitData();
	static void Localize();

public:
	mutable const wchar* m_pLocalizedName;
	mutable const wchar* m_pLocalizedDesc;
#endif
	static const PlayerClass* fromNums(int iTeam, int iClass); //for backwards-compatability with old numbering system
	static const PlayerClass* fromAbbreviation(int iTeam, const char* pszAbbreviation);
	bool GetLimitsAreInitialized() const { return m_pcvLimit_sml != NULL; }
	void InitLimits() const;
public:
	mutable ConVar* m_pcvLimit_lrg = nullptr;
	mutable ConVar* m_pcvLimit_med = nullptr;
	mutable ConVar* m_pcvLimit_sml = nullptr;
public:
	//int		GetLimitLrg() const { return m_pcvLimit_lrg->GetInt(); }
	//int		GetLimitMed() const { return m_pcvLimit_med->GetInt(); }
	//int		GetLimitSml() const { return m_pcvLimit_sml->GetInt(); }

	NClassQuota::SPopulationCounter* m_pPopCounter;
	//static int getClassLimit(const PlayerClass* pClass); //maximum number of players on the team who can use this class
	//static int numClasses(); //teams count individually, so American Infantry and British Infantry are separate classes
	//static int numModelsForTeam(int iTeam);
	//inline static int numClassesForTeam(int iTeam) { return iTeam == TEAM_AMERICANS ? TOTAL_AMER_CLASSES : TOTAL_BRIT_CLASSES; }
	//static const PlayerClass* const* asList(); //retrieves a list of pointers to the classes

	//static void RemoveClassLimits();
	static void UpdateClassSpeeds(); //updates class base speeds based on server variables
};

/*
EXPOSE INDIVIDUAL CLASSES TO REST OF PROGRAM
*/
namespace PlayerClasses {
#define dec(name) extern const PlayerClass* g_p##name;
		dec(BInfantry)
		dec(BOfficer)
		dec(BJaeger)
		dec(BNative)
		dec(BLinf)
		dec(BGrenadier)

		dec(AInfantry)
		dec(AOfficer)
		dec(AFrontiersman)
		dec(AMilitia)
		//dec(AStateMilitia)
		dec(AFrenchGre)

#undef dec

}

/*
PLAYER MODEL PATHS AND NAMES - these are used repeatedly for precacheing the models and assigning them to players
*/

#define MODEL_BINFANTRY			"models/player/british/infantry/br_infantry.mdl"
#define MODEL_BOFFICER			"models/player/british/officer/br_officer.mdl"
//#define MODEL_BOFFICER			"models/player/british/light_b/light_b.mdl"
#define MODEL_BJAEGER			"models/player/british/jager/jager.mdl"
#define MODEL_BNATIVE			"models/player/british/mohawk/mohawk.mdl"
#define MODEL_BLINF				"models/player/british/loyalist/loyalist.mdl"
#define MODEL_BNEWFOUNDLAND_INFANTRY				"models/player/british/newfoundland_infantry/newfoundland_infantry.mdl"
#define MODEL_BGRENADIER		"models/player/british/grenadier/br_grenadier.mdl"
#define MODEL_BGRENADIER_ALT		"models/player/british/grenadier/br_grenadier_alt.mdl"

#define MODEL_AINFANTRY			"models/player/american/infantry/american_infantry.mdl"
#define MODEL_AOFFICER			"models/player/american/light_a/light_a.mdl"
#define MODEL_AFRONTIERSMAN		"models/player/american/frontiersman/frontiersman.mdl"
#define MODEL_AMILITIA			"models/player/american/minuteman/minuteman.mdl"
#define MODEL_ASTATEMILITIA		"models/player/american/state/state_militia.mdl"
#define MODEL_AFRENCH			"models/player/american/french_gre/french_gre.mdl"
#define MODEL_AFRENCH_ALT		"models/player/american/french_gre/french_gre_alt.mdl"
#define MODEL_AFRENCHOFFICER	"models/player/american/french_officer/french_officer.mdl"

#define ARMS_REGULAR 0
#define ARMS_NATIVE 1
#define ARMS_BRIT_GRE 2
#define ARMS_FRE 3

#define SLEEVE_INNER_BLANK 0
#define SLEEVE_INNER_PLAIN 1
#define SLEEVE_INNER_LACE 2
