void MessageRatingsDb(const char* pMessage);

void escape_quotes(char* dest, const char* src, int len);

void RequestTop5(int ClientID);

void RequestRank(int ClientID, const char *pName);
