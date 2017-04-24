#include <base/system.h>
#include <engine/shared/config.h>
#include <engine/shared/protocol.h>

#include "gamecontext.h"
#include "ratedgame.h"

#include "reportscore.h"

CRatedGame::CRatedGame(class CGameContext *pGameServer)
{
	m_pGameServer = pGameServer;
	m_pServer = m_pGameServer->Server();
	m_IsRatedGame = false;
}

CRatedGame::~CRatedGame() { }

void CRatedGame::TryStartRatedGame(int Warmup)
{
	int PlayersNumber = 0;
	int RedTeamPlayers = 0;
	int BlueTeamPlayers = 0;
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (m_pGameServer->m_apPlayers[i] != NULL && m_pGameServer->m_apPlayers[i]->GetTeam() != TEAM_SPECTATORS)
		{
			PlayersNumber++;
			if (m_pGameServer->m_apPlayers[i]->GetTeam() == TEAM_RED)
				RedTeamPlayers++;
			else if (m_pGameServer->m_apPlayers[i]->GetTeam() == TEAM_BLUE)
				BlueTeamPlayers++;
		}
	}

	if(str_comp_nocase(g_Config.m_SvGametype, "rCTF") == 0)
	{
		if (PlayersNumber != 10 && PlayersNumber != 8)
		{
			m_pGameServer->SendChatTarget(-1, "Cannot start rated game. Only 4on4 and 5on5 games are supported.");
		}
		else if (RedTeamPlayers != BlueTeamPlayers)
		{
			m_pGameServer->SendChatTarget(-1, "Cannot start rated game. Teams are not balanced.");
		}
		else if (PlayersNumber == 10)
		{
			BeginRatedGame(1000, 60, Warmup < 0 ? 20 : Warmup);
		}
		else
		{
			BeginRatedGame(800, 40, Warmup < 0 ? 20 : Warmup);
		}
	}
	else if(str_comp_nocase(g_Config.m_SvGametype, "rTDM") == 0)
	{
		if (PlayersNumber != 2)
		{
			m_pGameServer->SendChatTarget(-1, "Cannot start rated game. Only 1on1 games are supported.");
		}
		else
		{
			BeginRatedGame(10, 7, Warmup < 0 ? 10 : Warmup);
		}
	}
	else
	{
		char aBuf[256];
		str_format(aBuf, sizeof(aBuf), "Rated games with gametype %s are not supported.", g_Config.m_SvGametype);
		m_pGameServer->SendChatTarget(-1, aBuf);
	}
}

const char* TeamToString(int Team)
{
	return TEAM_RED == Team ? "RED" : "BLUE";
}

void CRatedGame::SendGameinfo()
{
	char aBuf[1024];
	int GameTime = (Server()->Tick() - GameServer()->m_pController->m_RoundStartTick) / Server()->TickSpeed();
	int WinnerTeam = GameServer()->m_pController->m_aTeamscore[TEAM_RED] > GameServer()->m_pController->m_aTeamscore[TEAM_BLUE] ? TEAM_RED : TEAM_BLUE;
	str_format(aBuf, sizeof(aBuf), "Gameinfo\nGametype: %s\nMap: %s\nGametime: %d\nResult: %s\nPlayers:\n", GameServer()->m_pController->m_pGameType, g_Config.m_SvMap, GameTime, TeamToString(WinnerTeam));
	for(int c = 0; c < MAX_CLIENTS; c++)
	{
		CPlayer *pPlayer = GameServer()->m_apPlayers[c];
		if(!pPlayer)
			continue;

		char aPlayerBuf[128];

		if(pPlayer->GetTeam() != TEAM_SPECTATORS)
		{
			char aName[MAX_NAME_LENGTH * 2];
			escape_quotes(aName, Server()->ClientName(pPlayer->GetCID()), sizeof(aName));
			char aClan[MAX_CLAN_LENGTH * 2];
			escape_quotes(aClan, Server()->ClientClan(pPlayer->GetCID()), sizeof(aClan));
			str_format(aPlayerBuf, sizeof(aPlayerBuf), "\"%s\" \"%s\" %d %s\n", aName, aClan, pPlayer->m_Score, TeamToString(pPlayer->GetTeam()));
			str_append(aBuf, aPlayerBuf, sizeof(aBuf));
		}
	}
	char aLogBuf[1064];
	str_format(aLogBuf, sizeof(aLogBuf), "Reporting gameinfo:\n%s", aLogBuf);
	GameServer()->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aBuf);
	MessageRatingsDb(aBuf);
}

void CRatedGame::OnEndRound()
{
	if (!GameServer()->m_pRatedGame->IsRatedGame())
	{
		return;
	}
	SendGameinfo();
	EndRatedGame();
}

void CRatedGame::OnClientDrop(int ClientID)
{
	EndRatedGame();
}

void CRatedGame::BeginRatedGame(int Scorelimit, int Timelimit, int Warmup)
{
	m_IsRatedGame = true;
	g_Config.m_SvScorelimit = Scorelimit;
	g_Config.m_SvTimelimit = Timelimit;
	GameServer()->m_pController->DoWarmup(Warmup);
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "Starting rated game. Good luck!");
	GameServer()->SendChatTarget(-1, aBuf);
}

void CRatedGame::EndRatedGame()
{
	m_IsRatedGame = false;
	g_Config.m_SvScorelimit = 0;
	g_Config.m_SvTimelimit = 0;
}
