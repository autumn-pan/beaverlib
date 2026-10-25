#define _POSIX_C_SOURCE 200809L

#include "parser.h"
#include "tm.h"
#include "forward_sim.h"

#include <stdio.h>
#include <time.h>
#include <stdint.h>

int64_t get_current_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)(ts.tv_sec) * 1000 + (int64_t)(ts.tv_nsec) / 1000000;
}

int main(void) {
  Instruction **instructions = (parse_tm("1RB1LC_1RC1RB_1RD0LE_1LA1LD_---0LA_1RA1RA"));
  TuringMachine *tm = tm_new(instructions);


  int64_t c1 = get_current_time_ms();

  for(int i = 0; i < 100000000; i++)
  {
    if(tm_step(tm) == 0)
    {
      printf("index: %i", i);

      break;
    }

  } 

  int64_t c2 = get_current_time_ms();

  printf("diff: %llu", c2-c1);
  //save_tm(tm);
}
