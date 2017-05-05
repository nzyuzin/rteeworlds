#ifndef GAME_SERVER_PLAYERSTATS_H
#define GAME_SERVER_PLAYERSTATS_H
class CPlayerStats
{
	class CGameContext *m_pGameServer;
	class IServer *m_pServer;

	int m_ClientID;

	int m_HammerKills;
	int m_GunKills;
	int m_ShotgunKills;
	int m_GrenadeKills;
	int m_RifleKills;

	int m_Deaths;
	int m_Suicides;
	int m_FlagGrabs;
	int m_FlagCaptures;
	int m_FlagReturns;
	int m_FlagCarrierKills;

	protected:
	CGameContext *GameServer() const { return m_pGameServer; }
	IServer *Server() const {return m_pServer; }

	public:
	CPlayerStats(class CGameContext *pGameServer, int ClientID);
	~CPlayerStats();

	int HammerKills() const { return m_HammerKills; }
	int GunKills() const { return m_GunKills; }
	int ShotgunKills() const { return m_ShotgunKills; }
	int GrenadeKills() const { return m_GrenadeKills; }
	int RifleKills() const { return m_RifleKills; }

	int Deaths() const { return m_Deaths; }
	int Suicides() const { return m_Suicides; }
	int FlagGrabs() const { return m_FlagGrabs; }
	int FlagCaptures() const { return m_FlagCaptures; }
	int FlagReturns() const { return m_FlagReturns; }
	int FlagCarrierKills() const { return m_FlagCarrierKills; }

	void Reset();

	void OnDeath();

	void OnKill(int Target, int Weapon);
	void OnFlagCarrierKill();

	void OnFlagReturn();
	void OnFlagGrab();
	void OnFlagCapture();
};
#endif
