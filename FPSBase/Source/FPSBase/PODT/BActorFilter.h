#pragma once
#include "../Util.h"
#include "CoreMinimal.h"
#include "BActorFilter.generated.h"

USTRUCT()
struct FIActorFilter {
    GENERATED_BODY()
protected:
    uint8 m_iForTeam = TEAM_BOTH;
    FString m_sNameFilter = "";
    FString m_sClassNameFilter = "";
    bool m_bPlayerOnly = false;
    uint8 m_iRoleFilter = 0;

public:
    FIActorFilter();

    bool TestActor(AActor* pActor);

    //read-only
    uint8 GetTeamNumber() const { return m_iForTeam; }
};