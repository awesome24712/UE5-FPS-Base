#include "../Util.h"

enum EDamageType {
    DMG_GENERIC = BF(0);
    DMG_STAB = BF(1); //bayonets
    DMG_SLASH = BF(2);  //swords
    DMG_CRUSH = BF(3) //axes and clubs
    DMG_EXPLOSION = BF(4); //grenades, world explosions
    DMG_BULLET = BF(5); //get pointblanked
    DMG_STUN = BF(6); //add extra stun effect/debuff

    DMG_BURN = BF(7); //player on fire or near fire
    DMG_DROWN = BF(8); //underwater for too long
    DMG_GRAVITY = BF(9); //turn the gravity generators back on!

    DMG_BLEED = BF(10); //I need a medic!
}

class AActor;
class ABaseCharacter;
class WeaponDef;

struct DamageInfo {
private:
    int m_iDamage;
    uint32 m_iDamageTypes;

    AActor* m_pVictim;
    AActor* m_pAttacker;
    AActor* m_pWeapon;
    WeaponDef* m_pWeaponDef;

    int m_iHitGroup; //TODO decide hitgroups!

public:


    bool IsFatal();
    bool IsFriendlyFire();
}

class IDamageable {
private:

    int m_iHealth;
    int m_iIgnoredDamageTypes;


}