#include "gamecontext.h"
#include "reportscore.h"

bool IsInvalidClientID(int ClientID)
{
	return ClientID < 0 || ClientID >= MAX_CLIENTS;
}

void CGameContext::ConRank(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	int ClientID = pSelf->Server()->GetRconCID();
	if (IsInvalidClientID(ClientID))
		return;
	if (!pSelf->m_apPlayers[ClientID])
		return;
	const char *pTargetName;
	if (pResult->NumArguments() > 0)
	{
		pTargetName = pResult->GetString(0);
	}
	else
	{
		pTargetName = pSelf->Server()->ClientName(ClientID);
	}
	char RankRequest[256];
	str_format(RankRequest, sizeof(RankRequest), "Player_request\nPlayer_rank\n%d\n127.0.0.1:18383\n\"%s\"", ClientID, pTargetName);
	ReportGameinfo(RankRequest);
}

void CGameContext::ConTop5(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	int ClientID = pSelf->Server()->GetRconCID();
	if (IsInvalidClientID(ClientID))
		return;
	if (!pSelf->m_apPlayers[ClientID])
		return;
	char Top5Request[256];
	str_format(Top5Request, sizeof(Top5Request), "Player_request\nTop5_players\n%d\n127.0.0.1:18383", ClientID);
	ReportGameinfo(Top5Request);
}
