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
	RequestRank(ClientID, pTargetName);
}

void CGameContext::ConTop5(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	int ClientID = pSelf->Server()->GetRconCID();
	if (IsInvalidClientID(ClientID))
		return;
	if (!pSelf->m_apPlayers[ClientID])
		return;
	RequestTop5(ClientID);
}

void CGameContext::ConCurrentStats(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	int ClientID = pSelf->Server()->GetRconCID();
	if (IsInvalidClientID(ClientID))
		return;
	if (!pSelf->m_apPlayers[ClientID])
		return;

	pSelf->m_pRatedGame->ReportCurrentStats(ClientID);
}

void CGameContext::ConStats(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	int ClientID = pSelf->Server()->GetRconCID();
	if (IsInvalidClientID(ClientID))
		return;
	if (!pSelf->m_apPlayers[ClientID])
		return;

	pSelf->m_pRatedGame->ReportStats(ClientID);
}
