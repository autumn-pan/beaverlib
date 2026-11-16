#ifndef FSIM_H
#define FSIM_H

#include "tm.h"

#define FSIM_TAPE_LEN 65536

void run_fsim(Instruction **instructions);
void run_telemetric_fsim(Instruction **instructions);

#endif