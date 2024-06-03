//
// Created by sheshukovde on 01.06.2024.
//

#ifndef CPU_TEST_UTILS_H
#define CPU_TEST_UTILS_H

// max file name for log
#define FILE_LOG_NAME 256

typedef struct{
  int num_threads;            // num thread
  int show_help;              // setup to 1 if need show help
  int use_log;                // use log
  char fname[FILE_LOG_NAME];  // file name for log
}params_t;

// get time ms after start system
unsigned long get_tick_count();

// parse param arguments
int parse_param(int argc, char **argv, params_t *params);

// print help for usage
void usage();

// print logo
void logo();

// run test
void test(params_t *params);

#endif //CPU_TEST_UTILS_H
