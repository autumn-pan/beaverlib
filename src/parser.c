#include "parser.h"
#include "tm.h"
#include <stdio.h>
#include <stdlib.h>

Instruction **parse_tm(const char *line)
{
  Instruction **instructions = malloc(2*sizeof(Instruction *));

  for(int i = 0; i < 2; i++)
    instructions[i] = malloc(sizeof(Instruction) * 6);
  
  const char *ptr = line;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 2; j++) {
      if (*ptr == '_') {
        ptr++;
      }

      instructions[j][i].write = (*ptr == '1');
      ptr++;
      instructions[j][i].move = (*ptr == 'R');
      ptr++;
      instructions[j][i].next_state = *ptr - 'A';
      ptr++;

      instructions[j][i].req_symbol = j;
      instructions[j][i].req_state = i;
    }
  }

  return instructions;
}

void dump_instructions(Instruction **instructions) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) {
      Instruction inst = instructions[i][j];
      printf("Instruction %d,%d: write=%d, move=%d, next_state=%c\n", i, j, inst.write, inst.move, inst.next_state);
    }
  }
}