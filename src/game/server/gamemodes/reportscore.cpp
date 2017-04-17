#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "reportscore.h"

void ReportGameinfo(const char* GameinfoString)
{
  pid_t pid = fork();
  if (pid == 0)
  {
    execlp("./send_gameinfo", "./send_gameinfo", GameinfoString, NULL);
    if (errno != 0)
    {
      perror("error");
    }
  }
}
