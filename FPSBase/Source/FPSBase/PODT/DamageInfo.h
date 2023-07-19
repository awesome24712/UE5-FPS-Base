#pragma once
#include "../Util.h"
#include "CoreMinimal.h"
#include "DamageInfo.generated.h"

enum EDamageType : int {
    DMG_GENERIC     = BF(0),
    DMG_STAB        = BF(1), //bayonets
    DMG_SLASH       = BF(2),  //swords
    DMG_CRUSH       = BF(3), //axes and clubs
    DMG_EXPLOSION   = BF(4), //grenades, world explosions
    DMG_BULLET      = BF(5), //get pointblanked
    DMG_STUN        = BF(6), //add extra stun effect/debuff

    DMG_BURN        = BF(7), //player on fire or near fire
    DMG_DROWN       = BF(8), //underwater for too long
    DMG_GRAVITY     = BF(9), //turn the gravity generators back on!

    DMG_BLEED       = BF(10), //I need a medic!

    DMG_VOID        = BF(11), //Player fell out of map
    DMG_TRIGGER_DESERTER = BF(12), //For "You are abandoning the battle!" edge of map triggers
    DMG_TRIGGER     = BF(13), //Generic trigger damage

    DMG_NONFATAL    = BF(14), //take away health, but leave at least 1 hp
    DMG_FRIENDLY_OVERRIDE = BF(15) //damage which can apply even if the normal ff is off
};

class AActor;
class ABaseCharacter;
class AFPSBaseCharacter;
class WeaponDef;
struct FIDamageable;
class Faction;

USTRUCT()
struct FDamageInfo {
    GENERATED_BODY()

    friend struct FIDamageable;
private:
    int m_iDamage;
    uint32 m_iDamageTypes;

    FIDamageable* m_pVictim;
    AActor* m_pAttacker;
    AActor* m_pWeapon;
    WeaponDef* m_pWeaponDef;

    int m_bfHitGroup; //TODO decide hitgroups!

public:
    FDamageInfo() {}
    FDamageInfo(
        int iDamage,
        FIDamageable* pVictim,
        int damageTypes = DMG_GENERIC,
        AActor* pAttacker = NULL,
        AActor* pWeapon = NULL,
        WeaponDef* pWeaponDef = NULL
    );

    void ApplyToVictim();

    bool    IsNominallyFatal() const;
    bool    IsFriendlyFire() const;
    int     GetDamage() const { return m_iDamage; }
    int     GetDamageTypes() const { return m_iDamageTypes; }

    FIDamageable* GetVictim() const { return m_pVictim; }
    AActor* GetAttacker() const { return m_pAttacker; }
    AActor* GetWeapon() const { return m_pWeapon; }
    WeaponDef* GetWeaponDef() const { return m_pWeaponDef; }

    void SetDamage(int iDamage) { m_iDamage = iDamage; }
};

USTRUCT()
struct FIDamageable {
    GENERATED_BODY()
private:

    int m_iHealth;
    int m_iMaxHealth;
    int m_iIgnoredDamageTypes; //which damage types to ignore, ex. breakables ignoring trigger damage

public:
    FIDamageable(int health = 100, int maxHealth = 100, int ignoredDamageTypes = 0);

    inline int GetHealth() const { return m_iHealth; }
    inline int GetMaxHealth() const { return m_iMaxHealth; }

    inline void SetHealth(int health) { m_iHealth = health; } //bypasses overridable events

    inline bool IsDead() const { return m_iHealth <= 0; };
    inline bool isAlive() const { return m_iHealth > 0; };

    //extendable boolean for whether or not to receive a given damage
    virtual bool ShouldTakeDamage(const FDamageInfo& di);

    void TakeDamage(FDamageInfo& di, bool bSkipChecks = false); //does the math and calls events

    //Overridables called by TakeDamage(...)
    virtual void FilterDamage(FDamageInfo& di) {} //called first, gives chance to modify damage before receiving it
    virtual void OnTakeDamage(const FDamageInfo& di) {}
    virtual void OnDeath(const FDamageInfo& di) {}
    virtual void OnTakeDamageDeathBlocked(const FDamageInfo& di) {} //called when we receive non-fatal damage that would have otherwise killed us
    virtual void OnHealed(const FDamageInfo& di) {} //called when damage is negative
    virtual void OnStunned(const FDamageInfo& di) {} //called when hit with stun damage
};

UCLASS()
class UDamageEvents : public UObject {
    GENERATED_BODY()


    void DmgExplosion(
        const FVector& src,
        int damage,
        int radius,
        FIDamageable** ppIgnoredActors = NULL, //null-terminated list
        AActor* pAttacker = NULL,
        AActor* pWeapon = NULL,
        WeaponDef* pWeaponDef = NULL);

};