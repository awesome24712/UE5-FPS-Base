#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Util.h"
#include "Flag.generated.h"

class Faction;
class ABGPlayer;

UCLASS(config=Game)
class AFlag : public AActor, public IJsonBindable {
	GENERATED_BODY()

protected:
	//--------------------------------------------------------------
	// Bindable variables
	//--------------------------------------------------------------
	bool m_bStartDisabled = false; //also controls visibility

	uint8 m_iForTeam = TEAM_BOTH; //0 => neither, 1 => attacker, 2 => defender, 4 = both
	uint8 m_iStartingTeam = 0; //who owns flag at round start
	uint8 m_iPickupTeam = 0; //which team can pickup this flag?
	float m_flRadius = 2.f; //capture or pickup radius
	float m_flCaptureTime = 3.f;

	uint16 m_iCaptureBonus = 5; //number of points for capturing this flag
	uint16 m_iCapturePlayerBonus = 1;//points given to player capturing flag
	uint8 m_iCaptureBuffT1 = 0;
	uint8 m_iCaptureBuffT2 = 0;
	uint8 m_iPlayersForCapture = 2;

	//which textures to use for the flag
	uint8 m_iUnalignedMatIndex = 0; //TODO make more generic
	uint8 m_iT1MatIndex = 1;
	uint8 m_iT2MatIndex = 2;

	uint8 m_iHudSlot = 0; //where to show in HUD

	float m_flBotNoticeRange = 12.f; //range at which bots should spot this flag

	float m_flPointDeliveryInterval = 10.f;

	bool m_bOwnerBlocksCapture = true; //whether or not the flag can be held by a defender when attackers are within radius

#define JT_FLAG_BASE_BINDINGS() \
	JT_BIND_BOOLEAN(m_bStartDisabled, "startDisabled", false); \
	JT_BIND_BYTE(m_iForTeam, "forTeam", true); \
	JT_BIND_BYTE(m_iStartingTeam, "startingTeam", true); \
	JT_BIND_BYTE(m_iPickupTeam, "pickupTeam", true); \
	JT_BIND_FLOAT(m_flRadius, "radius", true); \
	JT_BIND_FLOAT(m_flCaptureTime, "captureTime", true); \
	JT_BIND_USHORT(m_iCaptureBonus, "captureBonus", true); \
	JT_BIND_USHORT(m_iCapturePlayerBonus, "capturePlayerBonus", true); \
	JT_BIND_BYTE(m_iCaptureBuffT1, "captureBuffAttacker", false); \
	JT_BIND_BYTE(m_iCaptureBuffT2, "captureBuffDefender", false); \
	JT_BIND_BYTE(m_iPlayersForCapture, "playersForCapture", false); \
	JT_BIND_BYTE(m_iUnalignedMatIndex, "unalignedFlagMat", true); \
	JT_BIND_BYTE(m_iT1MatIndex, "attackerFlagMat", true); \
	JT_BIND_BYTE(m_iT2MatIndex, "defenderFlagMat", true); \
	JT_BIND_BYTE(m_iHudSlot, "hudSlot", true); \
	JT_BIND_FLOAT(m_flPointDeliveryInterval, "pointDeliverInterval", true); \
	JT_BIND_FLOAT(m_flBotNoticeRange, "botNoticeRange", true); \
	JT_BIND_BOOLEAN(m_bOwnerBlocksCapture, "defenderBlocksCapture", true);

	//--------------------------------------------------------------
	// Non-bindable variables
	//--------------------------------------------------------------
	USkeletalMeshComponent* m_mesh; //flag model
	ftime m_flRemainingCaptureTime;
	ftime m_flNextTimedPointDelivery;
	uint8 m_iCurrentTeam;
	uint8 m_iCapturingTeam;
	//uint8 m_iOwnerCount;
	uint8 m_iCapturerCount;

public:
	AFlag() : AFlag(false) {}

	AFlag(bool bSkipBindings);

	virtual void BeginPlay();

	void Tick(float deltaSeconds) override;

	void TimedCaptureThink(float deltaSeconds);
	void CtfThink(float deltaSeconds);
	void TimedPointThink(float deltaSeconds);

	
	void CaptureByTeam(ETeamNumber team, const TArray<ABGPlayer*>& capturers);
	//void Decapture();

	bool IsEnabled() const { return IsActorTickEnabled(); }
	void SetEnabled(bool bEnabled);

	void PlayWaveSound(); //flag billowing in wind
	void PlayCaptureSound();
};

