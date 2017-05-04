#include "gamecontext.h"

CPlayerStats::CPlayerStats(class CGameContext *pGameServer)
{
	m_pGameServer = pGameServer;
	m_pServer = m_pGameServer->Server();
	Reset();
}

CPlayerStats::~CPlayerStats() { }

void CPlayerStats::Reset()
{
	m_Kills = 0;
	m_Deaths = 0;
	m_FlagGrabs = 0;
	m_FlagCaptures = 0;
	m_FlagReturns = 0;
	m_FlaggerKills = 0;
}

void CPlayerStats::OnDeath()
{
	m_Deaths++;
}

void CPlayerStats::OnKill(int Weapon, int Target)
{
	m_Kills++;
	m_HammerKills++;
}

void CPlayerStats::OnFlagReturn()
{
	m_FlagReturns++;
}

void CPlayerStats::OnFlagGrab()
{
	m_FlagGrabs++;
}

void CPlayerStats::OnFlagCapture()
{
	m_FlagCaptures++;
}
