#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "reportscore.h"

void ReportScore(const char* ScoreString)
{
  pid_t pid = fork();
  if (pid == 0)
  {
    int result = execlp("./report_scores", "./report_scores", ScoreString, NULL);
    if (errno != 0)
    {
      perror("error");
    }
  }
}
