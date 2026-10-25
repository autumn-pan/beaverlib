#ifndef FORWARD_SIM_H
#define FORWARD_SIM_H

#include "tm.h"

TuringMachine *tm_new(Instruction **trans_table);
void tm_step(TuringMachine *tm);
void dump_tm(TuringMachine *tm);
void save_tm(TuringMachine *tm);

#endif