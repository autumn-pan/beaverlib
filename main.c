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

void main() {
  Instruction **instructions = (parse_tm("1RB1RA_0LC1LE_1LD1LC_1LA0LB_1LF1RE_---0RA"));

  TuringMachine *tm = tm_new(instructions);


  int64_t c1 = get_current_time_ms();

  for(int i = 0; i < 1000000000; i++)
  {
    tm_step(tm);
  } 

  int64_t c2 = get_current_time_ms();

  printf("diff: %llu", c2-c1);
  //save_tm(tm);
}
