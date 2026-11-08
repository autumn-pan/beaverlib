#define _POSIX_C_SOURCE 200809L

#include "parser.h"
#include "tm.h"
#include "deciders/forward_sim.h"
#include "config.h"
#include "deciders/backward.h"

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

int64_t get_current_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)(ts.tv_sec) * 1000 + (int64_t)(ts.tv_nsec) / 1000000;
}


#ifndef BR_ENABLED
int main(void) {
  char src[50];

  printf("Enter a 6-state, 2-symbol Turing Machine: ");
  scanf("%s", &src); 

  Instruction **instructions = (parse_tm(src));
  TuringMachine *tm = tm_new(instructions);
  dump_instructions(instructions);

  size_t t1 = get_current_time_ms();
  for(int i = 0; i < 1000000000; i++)
  {
    tm_step(tm);
  } 
  
  size_t t2 = get_current_time_ms();
  printf("Time: %llu\n", t2-t1);
  printf("Ops per second: %llu\n", 1000000000000/(t2-t1));

  dump_tm(tm);
}
#else
int main(void) {
  char src[50];

  printf("Enter a 6-state, 2-symbol Turing Machine: ");
  scanf("%s", &src); 

  Instruction **instructions = (parse_tm(src));
  TuringMachine *tm = tm_new(instructions);
  
  DeciderBR *decider = br_decider_gen(tm);
  dump_br_table(decider);

  printf("\n\nForward Analysis:\n");

  size_t t1 = get_current_time_ms();
  for(int i = 0; i < 1000000000; i++)
  {
    tm_step(tm);
  } 
  
  size_t t2 = get_current_time_ms();
  printf("Time: %llu\n", t2-t1);
  printf("Ops per second: %llu\n", 1000000000000/(t2-t1));

  dump_tm(tm);
}

#endif