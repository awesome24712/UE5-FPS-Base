#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flag.generated.h"

class Faction;
class FPSBaseCharacter;

UCLASS(config=Game)
class AFlag : public AActor {
	GENERATED_BODY()

protected:
	bool m_bStartDisabled = false; //also controls visibility

	uint8 m_iForTeam = 0; //0 => both teams, 1 => team 1, 2 => team 2

	float m_flRadius = 128.f; //capture or pickup radius

	uint16 m_iCaptureBonus = 5; //number of points for capturing this flag
	uint16 m_iCapturePlayerBonus = 1;//points given to player capturing flag

	uint8 m_iCaptureBuffT1 = 0;
	uint8 m_iCaptureBuffT2 = 0;

	//which textures to use for the flag
	uint8 m_iUnalignedMatIndex; //TODO make more generic
	uint8 m_iT1MatIndex;
	uint8 m_iT2MatIndex;

	USkeletalMeshComponent* Mesh1P; //flag model
	
	uint8 m_iHudSlot; //where to show in HUD

	float m_flBotNoticeRange; //range at which bots should spot this flag

	bool m_bEnabled;

public:
	AFlag();

	virtual void BeginPlay();

	void Tick(float deltaSeconds) override {
		if (IsEnabled()) {
			Think();
		}
	} 

	virtual void Think() {}

	bool IsEnabled() const { return m_bEnabled; }
	void SetEnabled(bool bEnabled);

	void PlayWaveSound(); //flag billowing in wind
	void PlayCaptureSound();
};