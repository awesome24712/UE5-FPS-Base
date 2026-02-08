#include "BActorFilter.h"
#include "BInteractable.h"
#include "DamageInfo.h"
#include "../Util.h"


FIActorFilter::FIActorFilter() {

}

bool FIActorFilter::TestActor(AActor* pActor) {
	//assume true, then check otherwise

	if (!pActor) {
		NLogger::Fatal("FIActorFilter tested against NULL actor!");
		return false;
	}

	if (!m_sNameFilter.IsEmpty()) {
		FString name;
		pActor->GetName(name);
		if (m_sNameFilter.Compare(name) != 0) {
			return false;
		}
	}
	
	if (m_iForTeam != TEAM_BOTH) {
		const FIInteractable* pInteractable = dynamic_cast<FIInteractable*>(pActor);
		if (pInteractable && m_iForTeam != pInteractable->GetTeamNumber()) {
			return false;
		}
	}

	if (!m_sClassNameFilter.IsEmpty()) {
		const IJsonBindable* pBindable = dynamic_cast<IJsonBindable*>(pActor);
		if (pBindable && m_sClassNameFilter.Compare(pBindable->GetClassName()) != 0) {
			return false;
		}
	}

	return true;
}