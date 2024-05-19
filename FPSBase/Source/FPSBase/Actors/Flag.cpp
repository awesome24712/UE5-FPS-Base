#include "Flag.h"

AFlag::AFlag() {

}

void AFlag::BeginPlay() {
	PlayWaveSound();
}

void AFlag::SetEnabled(bool bEnabled) {
	//do nothing if nothing happens
	if (IsActorTickEnabled() == bEnabled) {
		return;
	}

	SetActorTickEnabled(bEnabled);

	//TODO toggle visibility


}

void AFlag::PlayWaveSound() {

}

void AFlag::PlayCaptureSound() {

}