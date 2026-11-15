#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "deciders/fsim.h"
#include "tm.h"
#include "parser.h"

void run_fsim(Instruction **instructions)
{
  uint64_t *tape = calloc(FSIM_TAPE_LEN, sizeof(uint64_t));

  uint64_t *fi = tape + (FSIM_TAPE_LEN >> 1);
  uint8_t si = 0; // cycles from 0 to 31
  uint8_t state = 0;

  for(int i = 0; i < 10; i++)
  {
    bool read = *fi & (1ULL << si);

    Instruction instr = instructions[read][state];
    if(instr.move)
    {
      si++;
      if(si == 32)
      {
        si = 0;
        fi++;
      }
    }
    else
    {
      if(si == 0)
      {
        si == 31;
        fi--;
      }
      else
        si--;
    }
  }
}