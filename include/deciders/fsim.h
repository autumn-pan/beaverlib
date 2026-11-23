#ifndef FSIM_H
#define FSIM_H

#include "tm.h"

#define FSIM_TAPE_LEN 65536

void run_fsim(Instruction **instructions, size_t steps);
void run_telemetric_fsim(Instruction **instructions, size_t steps);

void fsim2(Instruction **instructions);

#endif