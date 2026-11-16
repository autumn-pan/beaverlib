#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "deciders/fsim.h"
#include "tm.h"

void run_fsim(Instruction **instructions)
{
  uint64_t *tape = calloc(FSIM_TAPE_LEN, sizeof(uint64_t));

  uint64_t *fi = tape + (FSIM_TAPE_LEN >> 1);
  uint64_t si = 1ULL << 63; // secondary bitmask
  uint8_t state = 0;

  for(int i = 0; i < 1000000000; i++)
  {
    bool read = (*fi & si) != 0;

    Instruction instr = instructions[read][state];

    // Write the necessary value
    if(instr.write)
      *fi |= si;
    else
      *fi &= ~si;

    // Move the machine head
    if(instr.move)
    {
      si >>= 1;
      if(si == 0)
      {
        si = 1ULL << 63;
        fi++;
      }
    }
    else
    {
      si <<= 1;
      if(si == 0)
      {
        si = 1ULL;
        fi--;
      }
    }

    state = instr.next_state;
    if(state > 5)
    {
      printf("Halted in %llu!\n", i + 1);
      break;
    }
  }

  printf("Optimized Forward Sim Complete!");
}