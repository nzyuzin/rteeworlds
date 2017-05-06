void MessageRatingsDb(const char* pMessage);

void escape_quotes(char* dest, const char* src, int len);

void RequestTop5(int ClientID);

void RequestStats(int ClientID, const char *pName);

void RequestRank(int ClientID, const char *pName);

void RequestAuth(int ClientID, const char *pName, const char *pSecret);
