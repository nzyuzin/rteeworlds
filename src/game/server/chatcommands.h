CHAT_COMMAND("rank", "?r", CFGFLAG_CHAT|CFGFLAG_SERVER, ConRank, this, "Shows the rank of the player with name r (your rank with no argument)")
CHAT_COMMAND("top5", "", CFGFLAG_CHAT|CFGFLAG_SERVER, ConTop5, this, "Shows first 5 players with the highest rating")
CHAT_COMMAND("current_stats", "", CFGFLAG_CHAT|CFGFLAG_SERVER, ConCurrentStats, this, "Shows your current stats")
CHAT_COMMAND("stats", "?r", CFGFLAG_CHAT|CFGFLAG_SERVER, ConStats, this, "Shows the stats of the player with name r (your stats with no argument)")
