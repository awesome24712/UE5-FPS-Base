#include "DamageInfo.h"
#include "../FPSBaseCharacter.h"

DamageInfo::DamageInfo(
    int iDamage, 
    IDamageable* pVictim,
    int damageTypes = DMG_GENERIC,
    AActor* pAttacker = NULL,
    AActor* pWeapon = NULL, 
    WeaponDef* pWeaponDef = NULL
) {
    m_iDamage = iDamage;
    m_pVictim = pVictim;
    m_iDamageTypes = damageTypes;
    m_pAttacker = pAttacker;
    m_pWeapon = pWeapon;
    m_pWeaponDef = pWeaponDef;
}

void DamageInfo::ApplyToVictim() {
    pVictim->TakeDamage(*this);
}

bool DamageInfo::IsNominallyFatal() {
    return !(m_iDamageTypes & DMG_NONFATAL) && (m_pVictim->GetHealth() <= m_iDamage);
}

bool DamageInfo::IsFriendlyFire() {
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

IDamageable::IDamageable(int health, int maxHealth, int ignoredDamageTypes) {
    m_iHealth = health;
    m_iMaxHealth = maxHealth;
    m_iIgnoredDamageTypes = ignoredDamageTypes;
}


extern ConVar* gr_friendlyfire;
extern ConVar* gr_friendlyfire_grenades;
extern ConVar* gr_friendlyfire_cannon;
bool IDamageable::ShouldTakeDamage(const DamageInfo& di) {
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

void IDamageable::TakeDamage(DamageInfo& di) {
    //possibly ignore this damage
    if (!ShouldTakeDamage(di)) {
        return;
    }

    AFPSBaseCharacter* pVictim = dynamic_cast<AFPSBaseCharacter*>(m_pVictim);
    if (pVictim) {
        //TODO apply hitbox multipliers!
    }

    //actually subtract the damage
    m_iHealth -= di.GetDamage();

    //if we were healed, dispatch that event
    //and we can safely ignore everything else
    if (di.GetDamage( < 0)) {
        OnHealed(di);
        return;
    }

    //Dispatch damage event
    OnTakeDamage(di);

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
        OnDeath();
    }
}

