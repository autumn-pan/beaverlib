#ifndef PARSER_H
#define PARSER_H
#include "tm.h"

Instruction **parse_tm(const char *line);
void dump_instructions(Instruction **instructions);

#endif
