#include "BInteractable.h"
#include "BActorFilter.h"

FIInteractable::FIInteractable() {

}

bool FIInteractable::AttemptActivate(AActor* pUser) {
	if (IsActivatableBy(pUser)) {
		OnActivate(pUser);
		return true;
	}
	return false;
}

bool FIInteractable::IsActivatableBy(AActor* pActor) {
	if (!m_pFilter) {
		return true;
	}
	return m_pFilter->TestActor(pActor);
}