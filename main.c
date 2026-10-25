#include "parser.h"
#include "tm.h"
#include "forward_sim.h"

#include <stdio.h>

void main() {
  Instruction **instructions = (parse_tm("1LB1RA_0LC1LE_1LD1LC_1LA0LB_1LF1RE_---0RA"));

  TuringMachine *tm = tm_new(instructions);

  dump_tm(tm);
  for(int i = 0; i < 100; i++)
  {
    tm_step(tm);
  } 
  dump_tm(tm);

}
