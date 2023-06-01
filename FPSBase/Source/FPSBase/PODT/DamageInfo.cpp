#include "DamageInfo.h"
#include "../FPSBaseCharacter.h"
#include "BVector.h"
#include "EngineUtils.h"

FDamageInfo::FDamageInfo(
    int iDamage, 
    FIDamageable* pVictim,
    int damageTypes,
    AActor* pAttacker,
    AActor* pWeapon, 
    WeaponDef* pWeaponDef
) {
    m_iDamage = iDamage;
    m_pVictim = pVictim;
    m_iDamageTypes = damageTypes;
    m_pAttacker = pAttacker;
    m_pWeapon = pWeapon;
    m_pWeaponDef = pWeaponDef;
}

void FDamageInfo::ApplyToVictim() {
    m_pVictim->TakeDamage(*this);
}

bool FDamageInfo::IsNominallyFatal() const {
    return !(m_iDamageTypes & DMG_NONFATAL) && (m_pVictim->GetHealth() <= m_iDamage);
}

bool FDamageInfo::IsFriendlyFire() const {
    bool result = false;
    if (m_pAttacker) {
        AFPSBaseCharacter* pAttacker = dynamic_cast<AFPSBaseCharacter*>(m_pAttacker);
        AFPSBaseCharacter* pVictim = dynamic_cast<AFPSBaseCharacter*>(m_pVictim);
        if (pVictim && pAttacker) {
            //TODO access assigned factions
            //result = pVictim->GetFaction() == pAttacker->GetFaction();
        }
    }

    return result;
}

FIDamageable::FIDamageable(int health, int maxHealth, int ignoredDamageTypes) {
    m_iHealth = health;
    m_iMaxHealth = maxHealth;
    m_iIgnoredDamageTypes = ignoredDamageTypes;
}


extern ConVar* gr_friendlyfire;
extern ConVar* gr_friendlyfire_grenades;
extern ConVar* gr_friendlyfire_cannon;
bool FIDamageable::ShouldTakeDamage(const FDamageInfo& di) {
    //ignore if we're already dead
    if (IsDead()) {
        return false;
    }
    
    //check ff first
    bool ff = gr_friendlyfire->GetValueBool();
    if (!ff) {
        if (!di.IsFriendlyFire()) {
            goto postFFCheck;
        }
        bool ffg = gr_friendlyfire_grenades->GetValueBool();
        if (!ffg && !(di.GetDamageTypes() & DMG_FRIENDLY_OVERRIDE)) {
            return false;
        }
    }
postFFCheck:    
    int leftoverDamage = di.GetDamageTypes() & ~m_iIgnoredDamageTypes;
    leftoverDamage = leftoverDamage & ~(DMG_NONFATAL & DMG_FRIENDLY_OVERRIDE); //always ignore these special flags too
    return !!leftoverDamage;
}

void FIDamageable::TakeDamage(FDamageInfo& di) {
    //possibly ignore this damage
    if (!ShouldTakeDamage(di)) {
        return;
    }

    AFPSBaseCharacter* pVictim = dynamic_cast<AFPSBaseCharacter*>(di.GetVictim());
    if (pVictim) {
        //TODO apply hitbox multipliers!
    }

    //actually subtract the damage
    m_iHealth -= di.GetDamage();

    //if we were healed, dispatch that event
    //and we can safely ignore everything else
    if (di.GetDamage() < 0) {
        OnHealed(di);
        return;
    }

    //Dispatch damage event
    OnTakeDamage(di);

    //dispatch stun event if applicable
    if (!di.IsNominallyFatal() && (di.GetDamageTypes() & DMG_STUN)) {
        OnStunned(di);
    }

    //if damage is non-fatal, ensure we have at least 1 health
    if (di.GetDamageTypes() & DMG_NONFATAL) {
        m_iHealth = m_iHealth < 1 ? 1 : m_iHealth;
        if (m_iHealth == 1) {
            OnTakeDamageDeathBlocked(di); //TODO do we need this?
            return; //we're not dead so we can exit early
        }
    }

    //Dispatch death event if we're dead now
    if (IsDead()) {
        OnDeath(di);
    }
}

extern UWorld* g_pCurrentWorld;
void UDamageEvents::DmgExplosion(const FVector& src, int damage, int radius, FIDamageable** pIgnoredActors, AActor* pAttacker, AActor* pWeapon, WeaponDef* pWeaponDef) {
    int r2 = radius*radius; //using the square of the radius avoids using square roots later
    
    //iterate through all actors to find close enough actor
    //TODO make this more efficient?
    for (TActorIterator<AActor> itr(g_pCurrentWorld); itr; ++itr) {
        AActor* act = itr.operator->();

        FIDamageable* pVictim = dynamic_cast<FIDamageable*>(act);
        if (!pVictim)
            continue;
        
        //ensure we're close enough
        vec dist2 = UVectorTools::DistanceBetweenSqr(act, src);
        if (dist2 < r2)
            continue;


        //TODO trace from source to victim to check for obstacles
        //FHitResult trace;
        //call UTIL_TraceLine or bypass to make more efficient


        //if initial trace failed, try a tetrahedral pattern, top to bottom
        /*float tr = 5.f;
        FVector tetrahedron[6] = {
            src + FVector(tr, 0, 0),
            src + FVector(-tr, 0, 0),
            src + FVector(0, tr, 0),
            src + FVector(0, -tr, 0),
            src + FVector(0, 0, tr),
            src + FVector(0, 0, -tr),
        }
        for (int i = 0; i < 6; i++) {

        }*/

        //build damage object and apply it
        //calc damage amount based on distance compared to radius
        damage *= (dist2 / r2); //quadratic damage dropoff, in the real world it'd be cubic
        FDamageInfo di = FDamageInfo(
            damage,
            pVictim,
            DMG_EXPLOSION | DMG_STUN,
            pAttacker,
            pWeapon,
            pWeaponDef
        );
        di.ApplyToVictim();

        //TODO do a physics explosion too?
        
    }
}

