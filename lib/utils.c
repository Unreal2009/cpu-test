//
// Created by sheshukovde on 01.06.2024.
//

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <pthread.h>

#include "utils.h"
#include "rasp_tools.h"

#define MAX_NUM_THREAD 20

extern volatile sig_atomic_t stop_flag;

// Return the number of milliseconds since the system was turned on (tick)
unsigned long get_tick_count()
{
  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC, &ts);

  return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

int parse_param(int argc, char **argv, params_t *params)
{
  if(!argv || !params) return -1;
  memset(params, 0, sizeof(params_t));

  int opt;
  char opts[] = "t:l:h";
  int result = 0;
  // put ':' in the starting of the
  // string so that program can
  //distinguish between '?' and ':'
  while((opt = getopt(argc, argv, opts)) != -1)
  {
    switch(opt)
    {
      case 'h':
        params->show_help = 1;
        break;
      case 'l':
        if(strlen(optarg) == 0) result = -1;
        strncpy(params->fname, optarg, FILE_LOG_NAME - 1);
        params->use_log = 1;
        break;
      case 't':
        params->num_threads = atoi(optarg);
        if(params->num_threads < 0 || params->num_threads > MAX_NUM_THREAD)
        {
          printf("Threads number (-t) must be 0..%d\r\n", MAX_NUM_THREAD);
          result = -1;
        }
        break;
      case ':':
        result = -1;
        break;
      case '?':
        result = -1;
        break;
    }
    if(result == -1) return -1;
  }
  return 0;
}

void logo()
{
  system("clear");
  fflush(stdout);
  printf("\r\n*** CPU_TEST ***\r\n");
}

void usage()
{
  printf("Using 'cpu_test -h -t A -l B'\r\n");
  printf("  -h A - print this help\r\n");
  printf("  -t A - number threads 0..10\r\n");
  printf("  -l B - write log to file B\r\n");
  printf("  'cpu_test -t 4 -l out.log' - run 4 threads and write log to file 'out.log'\r\n");
  printf("  'cpu_test -t 4'            - run 4 threads no write to log\r\n");
}

// data for threads
uint32_t thread_data[MAX_NUM_THREAD];

// id threads
pthread_t thread[MAX_NUM_THREAD] = {0};

int cpu_test(params_t *params)
{
  printf("\r\n");
  int cnt = 0;

  FILE *fff = NULL;

  if(params->use_log)
  {
    if(!(fff = fopen(params->fname, "wb+")))
      printf("error on open log file '%s'\r\n", params->fname);
    else
      printf("log file is '%s'\r\n", params->fname);
  }

  while(!stop_flag)
  {
    uint32_t tmp = get_tick_count() + 1000;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%04d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("%010d %010d ", cnt++, tmp);
    fflush(stdout);

    unsigned long sum;
    for(int i = 0; i < params->num_threads; i++)
    {
//      printf(" %ld ", thread_data[i]);
      sum += thread_data[i];
    }
//    printf(" %ld ", sum);
    fflush(stdout);


    float temp = get_temp();
    printf(" temp=%.2f`C\r\n", temp);

    fflush(stdout);

    if(fff && params->use_log)
    {
      fprintf(fff, "%04d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
      fprintf(fff, " %d %d", cnt, tmp);
      fprintf(fff, " %.2f\r\n", temp);
    }

    while(tmp > get_tick_count());
  }

  if(fff)
  {
    printf("close log file...");
    fclose(fff);
    fflush(fff);
    printf("OK\r\n");
    fff = NULL;
  }

  return 0;
}

int running = 1;
uint32_t cnt = 0;

// test thread
void* threadFunc(void* thread_data)
{
  // stop thread
  uint32_t *count = (uint32_t*)thread_data;
  while(running) { (*count)++; };
  pthread_exit(0);
}

void test(params_t *params)
{
  if(!params) return;

  printf("test start, press ESC for stop...\r\n");
  printf("  threads : %d\r\n", params->num_threads);
  if(params->use_log)
  {
    printf("  file : %s\r\n", params->fname);
  }else
  {
    printf("  no log to file\r\n");
  }

  // create thread by ID and threadFunc
  // give to thread ptr to data
  for(int i = 0; i < params->num_threads; i++)
  {
    pthread_create(&thread[i], NULL, threadFunc, (void*)&thread_data[i]);
  }

  cpu_test(params);
  running = 0;

  // wait finish threads
  printf("wait finish thread ... ");
  for(int i = 0; i < params->num_threads; i++)
    pthread_join(thread[i], NULL);
  printf("OK\r\n");
}
