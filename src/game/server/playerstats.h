#ifndef GAME_SERVER_PLAYERSTATS_H
#define GAME_SERVER_PLAYERSTATS_H
class CPlayerStats
{
	class CGameContext *m_pGameServer;
	class IServer *m_pServer;

	int m_Kills;
	int m_HammerKills;
	int m_PistolKills;
	int m_ShotgunKills;
	int m_GrenadeKills;
	int m_LaserKills;

	int m_Deaths;
	int m_FlagGrabs;
	int m_FlagCaptures;
	int m_FlagReturns;
	int m_FlaggerKills;

	protected:
	CGameContext *GameServer() const { return m_pGameServer; }
	IServer *Server() const {return m_pServer; }

	public:
	CPlayerStats(class CGameContext *pGameServer);
	~CPlayerStats();

	void Reset();
	void OnDeath();
	void OnKill(int Weapon, int Target);
	void OnFlagReturn();
	void OnFlagGrab();
	void OnFlagCapture();
};
#endif
