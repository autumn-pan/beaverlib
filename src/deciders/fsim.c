#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdbit.h>

#include "deciders/fsim.h"
#include "tm.h"

void print_binary(size_t num)
{
  uint8_t num_bytes = sizeof(num)*8;

  for(int i = num_bytes - 1; i >= 0; i--)
  {
    size_t bit = (num >> i) & 1;

    printf("%i", bit);
  }
}

uint64_t get_msb(uint64_t n)
{
  if(n == 0)
    return 0;

  return stdc_bit_width(n) - 1;
}

void run_fsim(Instruction **instructions, size_t steps)
{
  uint64_t *tape = calloc(FSIM_TAPE_LEN, sizeof(uint64_t));
  uint64_t *fi = tape + (FSIM_TAPE_LEN >> 1);
  uint64_t si = 1ULL << 63; // secondary bitmask
  uint8_t state = 0;

  for(int i = 0; i < steps; i++)
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

void run_telemetric_fsim(Instruction **instructions, size_t steps)
{
  printf("Beginning telemetric fsim!\n");
  uint8_t *tape = calloc(FSIM_TAPE_LEN, sizeof(uint64_t));

  uint8_t *fi = tape + (FSIM_TAPE_LEN >> 1);
  uint8_t si = 1U << 7; // secondary bitmask
  uint8_t state = 0;
  size_t fi_index = FSIM_TAPE_LEN >> 1;

  for(int i = 0; i < steps; i++)
  {
    uint64_t msb = get_msb(si);
    printf("Step: %llu | ", i);
    for(int j = 0; j < FSIM_TAPE_LEN; j++)
    {
      if(tape[j] != 0)
      {
        for(int k = 7; k >= 0; k--)
        {
          uint8_t bit = (tape[j] >> k) & 1U;
          if(msb == k && fi_index == j)
            printf("[%c:%i]", state + 'A', bit);
          else
            printf("%i", bit);
        }
      }
    }

    printf("\n");
    
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
        si = 1U << 7;
        fi++;
        fi_index++;
      }
    }
    else
    {
      si <<= 1;
      if(si == 0)
      {
        si = 1U;
        fi--;
        fi_index--;
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

void gfsim_step(GenForwardSim *gfsim)
{
  // do its thing
}