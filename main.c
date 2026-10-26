#define _POSIX_C_SOURCE 200809L

#include "parser.h"
#include "tm.h"
#include "forward_sim.h"

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

int64_t get_current_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)(ts.tv_sec) * 1000 + (int64_t)(ts.tv_nsec) / 1000000;
}

int main(void) {

  char src[50];

  printf("Enter a 6-state, 2-symbol Turing Machine: ");
  scanf("%s", &src); 

  Instruction **instructions = (parse_tm(src));
  TuringMachine *tm = tm_new(instructions);

  for(int i = 0; i < 1000000; i++)
  {
    tm_step(tm);
  } 

  dump_tm(tm);
}