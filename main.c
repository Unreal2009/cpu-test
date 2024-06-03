#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "utils.h"

volatile sig_atomic_t stop_flag = 0;
char str[] = "get signal...\r\n";

static void sigintHandler(int sig) {
  stop_flag = 1;
  write(STDERR_FILENO, str, strlen(str));
}

#include "rasp_tools.h"

int main(int argc, char **argv)
{

  // Register signals
  signal(SIGINT, sigintHandler);

  logo();

  if(argc <= 1)
  {
    usage();
    return 0;
  }

  params_t params;
  if(parse_param(argc, argv, &params) != 0)
  {
    if(params.show_help == 1) usage();
    return 1;
  }

  if(params.show_help == 1) usage();

  test(&params);

  return 0;
}
