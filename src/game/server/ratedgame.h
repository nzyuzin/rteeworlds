#ifndef GAME_SERVER_RATEDGAME_H
#define GAME_SERVER_RATEDGAME_H
#include <engine/shared/protocol.h>
#include "playerstats.h"

class CRatedGame
{
	class CGameContext *m_pGameServer;
	class IServer *m_pServer;

	bool m_IsRatedGame;
	char m_apAuthedPlayers[MAX_CLIENTS][MAX_NAME_LENGTH];

protected:
	CPlayerStats *m_apPlayerStats[MAX_CLIENTS];

	CGameContext *GameServer() const { return m_pGameServer; }
	IServer *Server() const {return m_pServer; }

public:
	CRatedGame(class CGameContext *pGameServer);
	~CRatedGame();

	bool IsRatedGame() const { return m_IsRatedGame; }
	void TryStartRatedGame(int Warmup);
	void OnStartRound();
	void OnEndRound();
	void OnClientDrop(int ClientID);
	void OnAuthRequest(int ClientID, const char *pName, const char *pPassword);

	void BeginRatedGame(int Scorelimit, int Timelimit, int Warmup);
	void EndRatedGame();

	void AuthClient(int ClientID, const char* pName);
	void BadAuth(int ClientID);

	bool IsAuthed(int ClientID);

	void SendGameinfo();
	void PrintStats(int ClientID, const char* pStatsType, int TotalGames, int HammerKills, int GunKills, int ShotgunKills, int GrenadeKills, int RifleKills, int Deaths, int Suicides, int FlagGrabs, int FlagCaptures, int FlagReturns, int FlagCarrierKills);
	void ReportCurrentStats(int ClientID);
	void ReportStats(int ClientID);

	// stats
	void OnKill(int VictimID, int KillerID, int Weapon);
	void OnFlagCarrierKill(int KillerID);

	void OnFlagCapture(int ClientID);
	void OnFlagReturn(int ClientID);
	void OnFlagGrab(int ClientID);
};
#endif
