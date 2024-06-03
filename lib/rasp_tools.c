#include <stdio.h>

#include "rasp_tools.h"

// buffer size for work with console strings
#define BUFSIZE_LOCAL (1024 * 2)

float get_temp()
{
// system commend for get temperature
  char cmd[] = "vcgencmd measure_temp";
  char buf[BUFSIZE_LOCAL];

  FILE *ptr;

  float temp;

// run vcgencmd and get console output for parse
// CPU core temperature
  if ((ptr = popen(cmd, "r")) != NULL) {
    while (fgets(buf, BUFSIZE_LOCAL, ptr) != NULL)
    {
      sscanf(buf, "temp=%f", &temp);
    }
    (void) pclose(ptr);
  }
  return temp;
};
