#include "GamerulesSystem.h"

BG4Gamerules::BG4Gamerules() : PerFrameSystem("gamerules") {
	ResetScoresAndTimes();

	m_bPaused = false;
	m_iMapCycleIndex = 0;
}

void BG4Gamerules::Init() {

}

void BG4Gamerules::Tick(float deltaSecond) {
	if (m_bPaused) {
		PauseThink();
		return;
	}

	//thinking on every frame
	Think();

	//Per second
	if (m_flNextPerSecondThink < g_globals.time) {
		PerSecondThink();
		m_flNextPerSecondThink += 1;

		if (m_flNextDecisecondThink < g_globals.time) {
			PerDecisecondThink();
			m_flNextDecisecondThink += 1;
		}

		if (m_flNextTicketThink < g_globals.time) {
			TicketThink();
			m_flNextTicketThink = gr_tickets_interval.GetValueInt();
		}
	}
	
	if (m_bFullcapThink) {
		FullcapThink();
		m_bFullcapThink = false;
	}
}

void BG4Gamerules::Think() {

}

void BG4Gamerules::PerSecondThink() {
	if (m_flNextMapChange < g_globals.time) {

	}
}

void BG4Gamerules::PerDecisecondThink() {

}

void BG4Gamerules::FullcapThink() {
	if (!IsSkirmish()) {
		return;
	}

	int fullCapCheckResult = CheckForFullcap();
	if (fullCapCheckResult) {
		int winBonus = gr_fullcap_bonus.GetValueInt();
		Faction *pWinner, *pLoser;

		if (fullCapCheckResult == 1) {
			pWinner = Faction::GetAttacker();
			pLoser = Faction::GetDefender();
			m_iScoreA += winBonus;
		}
		else {
			pWinner = Faction::GetDefender();
			pLoser = Faction::GetAttacker();
			m_iScoreD += winBonus;
		}
	}
}

void BG4Gamerules::TicketThink() {

}

void BG4Gamerules::PauseThink() {

}

void BG4Gamerules::ResetScoresAndTimes() {
	m_iScoreA = m_iScoreD = m_iRoundWinsA = m_iRoundWinsD = 0;

	m_flGameStartTime = m_flRoundStartTime = g_globals.time;
}

void BG4Gamerules::RestartMap() {

}

void BG4Gamerules::RestartRound(bool bIsWin, bool bCycleRound) {

}

void BG4Gamerules::PerformWinTheatrics(ETeamNumber iWinner) {

}

void BG4Gamerules::SwapTeams() {

}

void BG4Gamerules::ScrambleTeams() {

}

void BG4Gamerules::DeliverPlayerScore(int amount, ABGPlayer* pPlayer) {

}

void BG4Gamerules::ChangemapDelayed(FString mapName, ftime delay) {
	sr_nextmap.SetValue(mapName);
	m_flNextMapChange = g_globals.time + delay;
}

void BG4Gamerules::SetRemainingRoundTime(ftime remainder) {
	//back-set the round start time, as if more time had passed than actually
	m_flRoundStartTime = g_globals.time - gr_rounds_timelimit.GetValueInt() + (gr_rounds_timelimit.GetValueInt() - remainder);
}

void BG4Gamerules::CalcNextMap() {
	//TODO make this proper
	const char* mapList[] = {
		"todo",
		"todo2"
	};

	int max = 2;

	m_iMapCycleIndex++;
	if (m_iMapCycleIndex > max) {
		m_iMapCycleIndex = 0;
		sr_nextmap.SetValue(mapList[m_iMapCycleIndex]);
	}
}

void BG4Gamerules::Pause() {
	m_bPaused = true;
}
void BG4Gamerules::Unpause() {
	m_bPaused = false;
}

int BG4Gamerules::CheckForFullcap() const {
	return 0;
}

int BG4Gamerules::CountAlivePlayersAndTickets(const Faction* pFaction) const {
	return 1;
}

void BG4Gamerules::OnPlayerKilled(ABGPlayer* pVictim, const FDamageInfo& info) {
	//tickets, points, death notifications
}

ftime BG4Gamerules::GetRemainingGameTime() const {
	ftime elapsedMinutes = (g_globals.time - m_flGameStartTime) / 60;

	return ((float)gr_timelimit.GetValueInt()) - elapsedMinutes;
}
ftime BG4Gamerules::GetRemainingRoundTime() const {
	ftime elapsedSeconds = (g_globals.time - m_flRoundStartTime);

	return ((float)gr_rounds_timelimit.GetValueInt()) - elapsedSeconds;
}