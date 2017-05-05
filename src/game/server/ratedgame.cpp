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
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		m_apAuthedPlayers[i][0] = '\0';
		m_apPlayerStats[i] = new CPlayerStats(pGameServer, i);
	}
}

CRatedGame::~CRatedGame()
{
	for (int i = 0; i < MAX_CLIENTS; i++)
		delete m_apPlayerStats[i];
}

void CRatedGame::TryStartRatedGame(int Warmup)
{
	int PlayersNumber = 0;
	int RedTeamPlayers = 0;
	int BlueTeamPlayers = 0;
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (m_pGameServer->m_apPlayers[i] != NULL && IsAuthed(i) && m_pGameServer->m_apPlayers[i]->GetTeam() != TEAM_SPECTATORS)
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
	for(int ClientID = 0; ClientID < MAX_CLIENTS; ClientID++)
	{
		CPlayer *pPlayer = GameServer()->m_apPlayers[ClientID];
		if(!pPlayer)
			continue;

		char aPlayerBuf[128];

		if(pPlayer->GetTeam() != TEAM_SPECTATORS && IsAuthed(ClientID))
		{
			char aName[MAX_NAME_LENGTH * 2];
			escape_quotes(aName, m_apAuthedPlayers[ClientID], sizeof(aName));
			char aClan[MAX_CLAN_LENGTH * 2];
			escape_quotes(aClan, Server()->ClientClan(pPlayer->GetCID()), sizeof(aClan));
			str_format(aPlayerBuf, sizeof(aPlayerBuf),
					"\"%s\" \"%s\" %d %s %d %d %d %d %d %d %d %d %d %d %d\n",
					aName,
					aClan,
					pPlayer->m_Score,
					TeamToString(pPlayer->GetTeam()),
					m_apPlayerStats[ClientID]->HammerKills(),
					m_apPlayerStats[ClientID]->GunKills(),
					m_apPlayerStats[ClientID]->ShotgunKills(),
					m_apPlayerStats[ClientID]->GrenadeKills(),
					m_apPlayerStats[ClientID]->RifleKills(),
					m_apPlayerStats[ClientID]->Deaths(),
					m_apPlayerStats[ClientID]->Suicides(),
					m_apPlayerStats[ClientID]->FlagGrabs(),
					m_apPlayerStats[ClientID]->FlagCaptures(),
					m_apPlayerStats[ClientID]->FlagReturns(),
					m_apPlayerStats[ClientID]->FlagCarrierKills()
			);
			str_append(aBuf, aPlayerBuf, sizeof(aBuf));
		}
	}
	char aLogBuf[1064];
	str_format(aLogBuf, sizeof(aLogBuf), "Reporting gameinfo:\n%s", aBuf);
	GameServer()->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aLogBuf);
	MessageRatingsDb(aBuf);
}

void CRatedGame::PrintStats(int ClientID, const char* pStatsType, int TotalGames, int HammerKills, int GunKills, int ShotgunKills, int GrenadeKills, int RifleKills, int Deaths, int Suicides, int FlagGrabs, int FlagCaptures, int FlagReturns, int FlagCarrierKills)
{
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "%s Stats", pStatsType);
	m_pGameServer->SendChatTarget(ClientID, aBuf);

	m_pGameServer->SendChatTarget(ClientID, "General stats:");
	int Kills = HammerKills + GunKills + ShotgunKills + GrenadeKills + RifleKills;
	str_format(aBuf, sizeof(aBuf), "Kills: %d, Deaths: %d, K/D: %d", Kills, Deaths, Kills / ((float) Deaths));
	m_pGameServer->SendChatTarget(ClientID, aBuf);
	if (TotalGames > 0)
	{
		m_pGameServer->SendChatTarget(ClientID, "Per game:");
		str_format(aBuf, sizeof(aBuf), "Kills: %d, Deaths: %d, K/D: %d", Kills / TotalGames, Deaths / TotalGames, Kills / ((float) Deaths));
		m_pGameServer->SendChatTarget(ClientID, aBuf);
	}

	m_pGameServer->SendChatTarget(ClientID, "Weapon kills:");
	str_format(aBuf, sizeof(aBuf), "Hammer: %d, Pistol: %d, Shotgun: %d, Grenades: %d, Laser: %d", HammerKills, GunKills, ShotgunKills, GrenadeKills, RifleKills);
	m_pGameServer->SendChatTarget(ClientID, aBuf);

	if (TotalGames > 0)
	{
		m_pGameServer->SendChatTarget(ClientID, "Per game:");
		str_format(aBuf, sizeof(aBuf), "Hammer: %d, Pistol: %d, Shotgun: %d, Grenades: %d, Laser: %d", HammerKills / TotalGames, GunKills / TotalGames, ShotgunKills / TotalGames, GrenadeKills / TotalGames, RifleKills / TotalGames);
		m_pGameServer->SendChatTarget(ClientID, aBuf);
	}

	if(str_comp_nocase(g_Config.m_SvGametype, "rCTF") == 0)
	{
		m_pGameServer->SendChatTarget(ClientID, "Flag stats:");
		str_format(aBuf, sizeof(aBuf), "Grabs: %d, Captures: %d, Returns: %d, Carrier kills: %d", FlagGrabs, FlagCaptures, FlagReturns, FlagCarrierKills);
		m_pGameServer->SendChatTarget(ClientID, aBuf);

		if (TotalGames > 0)
		{
			m_pGameServer->SendChatTarget(ClientID, "Per game:");
			str_format(aBuf, sizeof(aBuf), "Grabs: %d, Captures: %d, Returns: %d, Carrier kills: %d", FlagGrabs, FlagCaptures, FlagReturns, FlagCarrierKills);
			m_pGameServer->SendChatTarget(ClientID, aBuf);
		}
	}
}

void CRatedGame::ReportCurrentStats(int ClientID)
{
	PrintStats(ClientID, "Current game", 0, m_apPlayerStats[ClientID]->HammerKills(), m_apPlayerStats[ClientID]->GunKills(), m_apPlayerStats[ClientID]->ShotgunKills(), m_apPlayerStats[ClientID]->GrenadeKills(), m_apPlayerStats[ClientID]->RifleKills(), m_apPlayerStats[ClientID]->Deaths(), m_apPlayerStats[ClientID]->Suicides(), m_apPlayerStats[ClientID]->FlagGrabs(), m_apPlayerStats[ClientID]->FlagCaptures(), m_apPlayerStats[ClientID]->FlagReturns(), m_apPlayerStats[ClientID]->FlagCarrierKills());
}

void CRatedGame::ReportStats(int ClientID)
{

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

void CRatedGame::OnStartRound()
{
	if (!GameServer()->m_pRatedGame->IsRatedGame())
	{
		return;
	}
	for (int i = 0; i < MAX_CLIENTS; i++)
		m_apPlayerStats[i]->Reset();
}

void CRatedGame::OnClientDrop(int ClientID)
{
	if (IsRatedGame() && GameServer()->m_apPlayers[ClientID]->GetTeam() != TEAM_SPECTATORS)
	{
		m_apPlayerStats[ClientID]->Reset();
		EndRatedGame();
	}
	m_apAuthedPlayers[ClientID][0] = '\0';
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "Client %d deauthed", ClientID);
	GameServer()->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aBuf);
}

void CRatedGame::OnAuthRequest(int ClientID, const char *pName, const char *pPassword)
{
	if (IsAuthed(ClientID))
	{
		GameServer()->SendChatTarget(ClientID, "You are already authorised");
		return;
	}
	RequestAuth(ClientID, pName, pPassword);
}

void CRatedGame::AuthClient(int ClientID, const char *pName)
{
	str_copy(m_apAuthedPlayers[ClientID], pName, sizeof(m_apAuthedPlayers[ClientID]));
	GameServer()->SendChatTarget(ClientID, "Authorisation succeeded");
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "Client %d authed", ClientID);
	GameServer()->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aBuf);
}

bool CRatedGame::IsAuthed(int ClientID)
{
	return m_apAuthedPlayers[ClientID][0] != '\0';
}

void CRatedGame::BadAuth(int ClientID)
{
	GameServer()->SendChatTarget(ClientID, "Authorisation failed");
}

void CRatedGame::BeginRatedGame(int Scorelimit, int Timelimit, int Warmup)
{
	m_IsRatedGame = true;
	g_Config.m_SvScorelimit = Scorelimit;
	g_Config.m_SvTimelimit = Timelimit;
	GameServer()->m_pController->DoWarmup(Warmup);
	GameServer()->SendChatTarget(-1, "Starting rated game. Good luck!");
}

void CRatedGame::EndRatedGame()
{
	for (int i = 0; i < MAX_CLIENTS; i++)
		m_apPlayerStats[i]->Reset();
	m_IsRatedGame = false;
	g_Config.m_SvScorelimit = 0;
	g_Config.m_SvTimelimit = 0;
}

void CRatedGame::OnKill(int VictimID, int KillerID, int Weapon)
{
	if (!IsRatedGame())
		return;

	m_apPlayerStats[VictimID]->OnDeath();
	m_apPlayerStats[KillerID]->OnKill(VictimID, Weapon);
}

void CRatedGame::OnFlagCarrierKill(int KillerID)
{
	if (!IsRatedGame())
		return;

	m_apPlayerStats[KillerID]->OnFlagCarrierKill();
}

void CRatedGame::OnFlagCapture(int ClientID)
{
	if (!IsRatedGame())
		return;

	m_apPlayerStats[ClientID]->OnFlagCapture();
}

void CRatedGame::OnFlagReturn(int ClientID)
{
	if (!IsRatedGame())
		return;

	m_apPlayerStats[ClientID]->OnFlagReturn();
}

void CRatedGame::OnFlagGrab(int ClientID)
{
	if (!IsRatedGame())
		return;

	m_apPlayerStats[ClientID]->OnFlagGrab();
}
