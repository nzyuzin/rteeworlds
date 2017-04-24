#ifndef GAME_SERVER_RATEDGAME_H
#define GAME_SERVER_RATEDGAME_H

class CRatedGame
{
	class CGameContext *m_pGameServer;
	class IServer *m_pServer;

	bool m_IsRatedGame;

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

	void SendGameinfo();
};
#endif
