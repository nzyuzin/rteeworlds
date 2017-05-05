#include <game/generated/protocol.h>
#include "gamecontext.h"

CPlayerStats::CPlayerStats(class CGameContext *pGameServer, int ClientID)
{
	m_pGameServer = pGameServer;
	m_pServer = m_pGameServer->Server();
	m_ClientID = ClientID;
	Reset();
}

CPlayerStats::~CPlayerStats() { }

void CPlayerStats::Reset()
{
	m_Kills = 0;
	m_HammerKills = 0;
	m_GunKills = 0;
	m_ShotgunKills = 0;
	m_RifleKills = 0;

	m_Deaths = 0;
	m_Suicides = 0;

	m_FlagGrabs = 0;
	m_FlagCaptures = 0;
	m_FlagReturns = 0;
	m_FlaggerKills = 0;
}

void CPlayerStats::OnDeath()
{
	m_Deaths++;
}

void CPlayerStats::OnKill(int Target, int Weapon)
{
	m_Kills++;
	if (Weapon == WEAPON_HAMMER)
		m_HammerKills++;
	else if (Weapon == WEAPON_GUN)
		m_GunKills++;
	else if (Weapon == WEAPON_SHOTGUN)
		m_ShotgunKills++;
	else if (Weapon == WEAPON_GRENADE)
		m_GrenadeKills++;
	else if (Weapon == WEAPON_RIFLE)
		m_RifleKills++;
	else if (Target == m_ClientID) // WEAPONS GAME, SELF, WORLD -- entities/character.h
		m_Suicides++;
}

void CPlayerStats::OnFlaggerKill()
{
	m_FlaggerKills++;
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
