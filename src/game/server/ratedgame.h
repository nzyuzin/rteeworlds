#ifndef GAME_SERVER_RATEDGAME_H
#define GAME_SERVER_RATEDGAME_H
#include <engine/shared/protocol.h>

class CRatedGame
{
	class CGameContext *m_pGameServer;
	class IServer *m_pServer;

	bool m_IsRatedGame;
	const char *m_apAuthedPlayers[MAX_CLIENTS];

protected:
	CGameContext *GameServer() const { return m_pGameServer; }
	IServer *Server() const {return m_pServer; }

public:
	CRatedGame(class CGameContext *pGameServer);
	~CRatedGame();

	bool IsRatedGame() const { return m_IsRatedGame; }
	void TryStartRatedGame(int Warmup);
	void OnEndRound();
	void OnClientDrop(int ClientID);
	void BeginRatedGame(int Scorelimit, int Timelimit, int Warmup);
	void EndRatedGame();

	void AuthClient(int ClientID, const char* pName);
	void BadAuth(int ClientID);

	bool IsAuthed(int ClientID);

	void SendGameinfo();
};
#endif
