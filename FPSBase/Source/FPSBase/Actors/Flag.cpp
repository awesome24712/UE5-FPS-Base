#include "Flag.h"
#include "../CommonHeaders.h"

AFlag::AFlag(bool bSkipBindings) {

	if (!bSkipBindings) {
		JT_START_BINDING_UCLASS("flag", AFlag);
		JT_FLAG_BASE_BINDINGS(); //bindings shared with CtfFlag
		JT_FINISH_BINDING();
	}
}

void AFlag::BeginPlay() {
	PlayWaveSound();
}

void AFlag::Tick(float deltaSeconds) {

	//if we don't have a radius, we're not a timed capture flag
	//also make us uncapturable 
	if (m_flRadius) {
		TimedCaptureThink(deltaSeconds);
	}
	if (m_iPickupTeam) {
		CtfThink(deltaSeconds);
	}
}

void AFlag::TimedCaptureThink(float deltaSeconds) {
	//three states
	// 1. a team already holds this flag
	// 2. a team is capturing this flag
	// 3. no one is capturing this flag (yet)
	//but first, if we're a one-time capture and already captured, just skip to points
	if (m_iCurrentTeam == m_iForTeam) {
		TimedPointThink(deltaSeconds);
		return;
	}


	//...if a team already owns this flag
	if (m_iCurrentTeam) {
		//if the owning players can block capture
		if (m_bOwnerBlocksCapture || m_iForTeam == TEAM_BOTH) {
			TArray<ABGPlayer*> capturers;
			TArray<ABGPlayer*> owners;
			GetPlayersInRadius(
				GetPlayersOfTeam(GetOtherTeam((ETeamNumber)m_iCurrentTeam)), 
				capturers, 
				GetActorLocation(), 
				m_flRadius);
			GetPlayersInRadius(
				GetPlayersOfTeam((ETeamNumber)m_iCurrentTeam), 
				owners, 
				GetActorLocation(), 
				m_flRadius);

			if (owners.Num() || capturers.Num() == 0) {
				//no changes, just think about points
				TimedPointThink(deltaSeconds);
			}
			else if ((!m_bOwnerBlocksCapture || owners.Num() == 0) 
				&& capturers.Num() >= owners.Num()) {
				
				if (capturers.Num() >= m_iPlayersForCapture) {
					//start capture!
					m_iCapturerCount = capturers.Num();
					m_iCapturingTeam = GetOtherTeam((ETeamNumber)m_iCurrentTeam);
					m_flRemainingCaptureTime = m_flCaptureTime;
				}

				//decapture, in any case!
				m_iCurrentTeam = TEAM_UNASSIGNED;
			}
		}
		else {
			TArray<ABGPlayer*> capturers;
			GetPlayersInRadius(
				GetPlayersOfTeam(GetOtherTeam((ETeamNumber)m_iCurrentTeam)),
				capturers,
				GetActorLocation(),
				m_flRadius);
			if (capturers.Num() == 0) {
				//no changes, just think about points
				TimedPointThink(deltaSeconds);
			}
			else if (capturers.Num() >= 1) {

				if (capturers.Num() >= m_iPlayersForCapture) {
					//start capture!
					m_iCapturerCount = capturers.Num();
					m_iCapturingTeam = GetOtherTeam((ETeamNumber)m_iCurrentTeam);
					m_flRemainingCaptureTime = m_flCaptureTime;
				}

				//decapture, in any case!
				m_iCurrentTeam = TEAM_UNASSIGNED;
			}
		}
	}


	//a team is atttempting to capture the flag
	else if (m_iCapturingTeam) {
		//ensure the capturing can continue, otherwise cancel it and return to
		TArray<ABGPlayer*> capturers;
		TArray<ABGPlayer*> others;
		GetPlayersInRadius(
			GetPlayersOfTeam((ETeamNumber)m_iCapturingTeam),
			capturers,
			GetActorLocation(),
			m_flRadius);
		if (m_iForTeam == TEAM_BOTH) {
			GetPlayersInRadius(
				GetPlayersOfTeam(GetOtherTeam((ETeamNumber)m_iCapturingTeam)),
				others,
				GetActorLocation(),
				m_flRadius);
		}
		
		//cancel the capture attempt if necessary
		if (others.Num() >= capturers.Num()
			|| capturers.Num() < m_iPlayersForCapture) {
			m_iCapturingTeam = TEAM_UNASSIGNED;
			m_iCapturerCount = 0;
		}
		//check if enough time has passed to complete the capture
		if (m_flRemainingCaptureTime < 0) {
			CaptureByTeam((ETeamNumber)m_iCapturingTeam, capturers);
		}
		else {
			m_flRemainingCaptureTime -= deltaSeconds;
		}
	}




	//no team owns this flag
	else {
		//if either team can capture this flag
		if (m_iForTeam == TEAM_BOTH) {
			TArray<ABGPlayer*> t1;
			TArray<ABGPlayer*> t2;
			GetPlayersInRadius(
				GetPlayersOfTeam(TEAM_ATTACKER),
				t1,
				GetActorLocation(),
				m_flRadius);
			GetPlayersInRadius(
				GetPlayersOfTeam(TEAM_DEFENDER),
				t2,
				GetActorLocation(),
				m_flRadius);

			//if both are zero or otherwise equal, nothing else to do
			if (t1.Num() == t2.Num()) {
				return;
			}
			ETeamNumber greaterTeam;
			uint8 greaterCount;
			if (t1.Num() > t2.Num()) {
				greaterTeam = TEAM_ATTACKER;
				greaterCount = t1.Num();
			}
			else {
				greaterTeam = TEAM_DEFENDER;
				greaterCount = t2.Num();
			}

			//if we have enough to start capture, start capture!
			if (greaterCount > m_iPlayersForCapture) {
				//start capture!
				m_iCapturerCount = greaterCount;
				m_iCapturingTeam = greaterTeam;
				m_flRemainingCaptureTime = m_flCaptureTime;
			}
		}
		//if only one team can capture this flag
		else {
			TArray<ABGPlayer*> t;
			GetPlayersInRadius(
				GetPlayersOfTeam((ETeamNumber) m_iForTeam),
				t,
				GetActorLocation(),
				m_flRadius);
			if (t.Num() > m_iPlayersForCapture) {
				//start capture!
				m_iCapturerCount = t.Num();
				m_iCapturingTeam = m_iForTeam;
				m_flRemainingCaptureTime = m_flCaptureTime;
			}
		}
	}


}

void AFlag::CtfThink(float deltaSeconds) {

}

void AFlag::TimedPointThink(float deltaSeconds) {
	if (m_flNextTimedPointDelivery < 0) {
		g_pGamerules->DeliverTeamScore(m_iCaptureBonus, (ETeamNumber)m_iCurrentTeam);
		m_flNextTimedPointDelivery = m_flPointDeliveryInterval;
	}
	
	m_flNextTimedPointDelivery -= deltaSeconds;
}

void AFlag::CaptureByTeam(ETeamNumber team, const TArray<ABGPlayer*>& capturers) {
	m_iCurrentTeam = team;
	m_iCapturingTeam = TEAM_UNASSIGNED;
	m_flRemainingCaptureTime = 0;

	g_pGamerules->DeliverTeamScore(m_iCaptureBonus, team);
	g_pGamerules->MarkFullcapCheckNextFrame(); //check for fullcaps

	//send per-player points
	for (auto p : capturers) {
		g_pGamerules->DeliverPlayerScore(m_iCaptureBonus, p);
	}

	PlayCaptureSound();
}

void AFlag::SetEnabled(bool bEnabled) {
	//do nothing if nothing happens
	if (IsActorTickEnabled() == bEnabled) {
		return;
	}

	SetActorTickEnabled(bEnabled);

	//TODO toggle visibility
	this->SetActorHiddenInGame(!bEnabled);

}

void AFlag::PlayWaveSound() {

}

void AFlag::PlayCaptureSound() {

}
