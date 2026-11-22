#ifndef FSIM_H
#define FSIM_H

#include "tm.h"

#define FSIM_TAPE_LEN 65536

typedef struct {
  Instruction *instructions;
  uint64_t tape;
  uint64_t *fi;
  uint64_t si;
  uint8_t state;
} GenForwardSim;

void run_fsim(Instruction **instructions, size_t steps);
void run_telemetric_fsim(Instruction **instructions, size_t steps);

#endif