#include "Flag.h"

AFlag::AFlag() {

}

void AFlag::BeginPlay() {
	PlayWaveSound();
}

void AFlag::SetEnabled(bool bEnabled) {
	//do nothing if nothing happens
	if (m_bEnabled == bEnabled) {
		return;
	}

	m_bEnabled = bEnabled;


}

void AFlag::PlayWaveSound() {

}

void AFlag::PlayCaptureSound() {

}