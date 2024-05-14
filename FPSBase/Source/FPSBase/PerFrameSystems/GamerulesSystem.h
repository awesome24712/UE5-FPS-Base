#pragma once
#include "CoreMinimal.h"
#include "../Util.h"
#include "PerFrameSystemManager.h"
#include "../Systems/FactionSystem.h"
#include "../PODT/DamageInfo.h"


inline bool IsSkirmish() { return gr_gamemode.GetValueInt() == 0; }
inline bool IsUsingTickets() { return gr_gamemode.GetValueInt() == 1; }
inline bool IsLMS() { return gr_gamemode.GetValueInt() == 2; }
inline bool IsLinebattle() { return gr_gamemode.GetValueInt() == 3; }
inline bool UsingRoundSystem() { return gr_rounds.GetValueInt() > 0; }

class ABGPlayer;

class BG4Gamerules;
extern BG4Gamerules g_pGamerules;

class BG4Gamerules : PerFrameSystem {
private:
	int m_iScoreA;
	int m_iScoreD;

	int m_iRoundWinsA;
	int m_iRoundWinsD;

	ftime m_flGameStartTime; //defaults to 0 on map change, but restartmap can override
	ftime m_flRoundStartTime;

	ftime m_flNextMapChange;

	bool m_bPaused;

	int m_iMapCycleIndex;

	//Overrides
	BG4Gamerules();
	void Init() override;
	void Tick(float deltaSecond) override;

	//Interval-based thinks
	float m_flNextPerSecondThink;
	float m_flNextDecisecondThink;
	bool m_bFullcapThink;
	float m_flNextTicketThink;
	void Think();
	void PerSecondThink();
	void PerDecisecondThink();
	void FullcapThink();
	void TicketThink();
	void PauseThink();

	//Procedures
	void ResetScoresAndTimes();
	void RestartMap();
	void RestartRound(bool bIsWin, bool bCycleRound);
	void PerformWinTheatrics(ETeamNumber iWinner);
	void SwapTeams();
	void ScrambleTeams();
	void DeliverPlayerScore(int amount, ABGPlayer* pPlayer);
	void MarkFullcapCheckNextFrame() { m_bFullcapThink = true; }
	void ChangemapDelayed(FString mapName, ftime delay);
	void SetRemainingRoundTime(ftime remainder);
	void CalcNextMap();

	void Pause();
	void Unpause();

	//Helpers
	int CheckForFullcap() const; //0 = none, 1 = attackers, 2 = defenders
	int CountAlivePlayersAndTickets(const Faction* pFaction) const;

	//Listeners
	void OnPlayerKilled(ABGPlayer* pVictim, const FDamageInfo& info);

public:
	//Getters - public
	int GetScoreA() const { return m_iScoreA; }
	int GetScoreD() const { return m_iScoreD; }
	int GetRoundsWonA() const { return m_iRoundWinsA; }
	int GetRoundsWonD() const { return m_iRoundWinsD; }
	ftime GetRemainingGameTime() const;
	ftime GetRemainingRoundTime() const;

	bool IsPaused() const { return m_bPaused; };
};