#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <base/system.h>
#include <engine/shared/config.h>
#include <engine/shared/protocol.h>

#include "reportscore.h"

void escape_quotes(char* dest, const char* src, int len)
{
  char c;
	int i = 0;
  while ((c = *(src++)) && i < len) {
		i++;
    switch(c) {
      case '\"':
        *(dest++) = '\\';
        *(dest++) = '\"';
        break;
      default:
        *(dest++) = c;
     }
  }
  *dest = '\0';
}

void PlayerRequestMessage(char *pResult, size_t Size, const char *pType, int ClientID, const char *pContent)
{
	str_format(pResult, Size, "Player_request\n%s\n%d\n127.0.0.1:%d\n\"%s\"", pType, ClientID, g_Config.m_EcPort, pContent);
}

void PlayerRankRequestMessage(char *pResult, size_t Size, int ClientID, const char *pName)
{
	char aEscapedName[MAX_NAME_LENGTH * 2];
	escape_quotes(aEscapedName, pName, sizeof(aEscapedName));
	PlayerRequestMessage(pResult, Size, "Player_rank", ClientID, aEscapedName);
}

void Top5RequestMessage(char *pResult, size_t Size, int ClientID)
{
	PlayerRequestMessage(pResult, Size, "Top5_players", ClientID, "");
}

void RequestRank(int ClientID, const char *pName)
{
	char aBuf[512];
	PlayerRankRequestMessage(aBuf, sizeof(aBuf), ClientID, pName);
	MessageRatingsDb(aBuf);
}

void RequestTop5(int ClientID)
{
	char aBuf[512];
	Top5RequestMessage(aBuf, sizeof(aBuf), ClientID);
	MessageRatingsDb(aBuf);
}

void MessageRatingsDb(const char *pMessage)
{
  pid_t pid = fork();
  if (pid == 0)
  {
    execlp("./send_gameinfo", "./send_gameinfo", pMessage, NULL);
    if (errno != 0)
    {
      perror("error");
    }
  }
}
