#pragma once
#include "../Util.h"
#include "CoreMinimal.h"
#include "BInteractable.generated.h"

struct FIActorFilter;

USTRUCT()
struct FIInteractable {
    GENERATED_BODY()
protected:
    uint8 m_iTeamNumber = TEAM_BOTH;
    bool m_bStartDisabled = false;
    bool m_bDisabled = false;
    FIActorFilter* m_pFilter;

public:
    FIInteractable();

    bool AttemptActivate(AActor* pUser); //returns true if successful
    virtual void OnActivate(AActor* pUser) {};

    //setters/muters
    void SetStartDisabled(bool b) { m_bStartDisabled = b; }
    void SetEnabled(bool b) { m_bDisabled = b; OnSetEnabled(m_bDisabled); }
    void ToggleEnabled() { m_bDisabled = !m_bDisabled; OnSetEnabled(m_bDisabled);}
    void SetFilter(FIActorFilter* pFilter) { m_pFilter; }
    virtual void OnSetEnabled(const bool& b) {};


    //read-only
    bool IsDisabledAtStart() const { return m_bStartDisabled; }
    bool IsDisabled() const { return m_bDisabled; }
    uint8 GetTeamNumber() const { return m_iTeamNumber; }
    bool IsActivatableBy(AActor* pActor);
};