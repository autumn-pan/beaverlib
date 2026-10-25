#include <stdio.h>
#include <stdlib.h>

#include "tm.h"

TuringMachine *tm_new(Instruction **trans_table) 
{
  TuringMachine *tm = malloc(sizeof(TuringMachine));
  if(!tm) 
  {
    return NULL;
  }

  // Since we use calloc, the tapes already read 0 on every cell
  tm->left_tape = calloc(HALF_TAPE_SIZE, sizeof(uint64_t));
  tm->right_tape = calloc(HALF_TAPE_SIZE, sizeof(uint64_t));

  tm->left_tape_size = HALF_TAPE_SIZE;
  tm->right_tape_size = HALF_TAPE_SIZE;

  tm->head_dir = 1;
  tm->head_pos = 0;

  tm->state = 'A';
  
  tm->trans_table = trans_table;

  return tm;
}

// append an additional uint64_t to a half-tape
void extend_half_tape(uint64_t *tape)
{
  size_t tape_size = sizeof(tape)/sizeof(uint64_t);
  tape = realloc(tape, sizeof(tape) + sizeof(uint64_t));

  tape[tape_size] = 0;
}

// append an additional uint64_t to the left half-tape
void extend_left_tape(TuringMachine* tm)
{
  extend_half_tape(tm->left_tape);
  tm->left_tape_size++;
}

// append an additional uint64_t to a right half-tape
void extend_right_tape(TuringMachine* tm)
{
  extend_half_tape(tm->right_tape);
  tm->right_tape_size++;
}

inline bool read(TuringMachine *tm) 
{
  uint64_t *half_tape = tm->right_tape;

  if(!tm->head_dir)
    half_tape = tm->left_tape;

  return 1U & (half_tape[tm->head_pos/64] >> (tm->head_pos % 64));
}

inline void write(TuringMachine *tm, bool value) 
{
  if(!tm->head_dir)
  {
    if(value)
      tm->left_tape[tm->head_pos/64] |= (1 << (tm->head_pos % 64));
    else
      tm->left_tape[tm->head_pos/64] &= ~(1 << (tm->head_pos % 64));
  }
  else
  {
    if(value)
      tm->right_tape[tm->head_pos/64] |= (1 << (tm->head_pos % 64));
    else
      tm->right_tape[tm->head_pos/64] &= ~(1 << (tm->head_pos % 64));
  }
}

inline void move(TuringMachine *tm, bool dir)
{
  // TODO: optimize this
  if(tm->head_dir == 1)
  {
    if(dir)
      tm->head_pos++;
    else if(!dir && tm->head_pos == 0)
        tm->head_dir = 0;
    else
      tm->head_pos--;
  }
  else
  {
    if(dir && tm->head_pos > 0)
      tm->head_pos--;
    else if(dir && tm->head_pos == 0)
      tm->head_dir = 1;
    else
      tm->head_pos++;
  }
}

void tm_step(TuringMachine *tm)
{
  bool read_value = read(tm);

  Instruction instr = tm->trans_table[read_value][tm->state - 'A'];
  write(tm, instr.write);
  move(tm, instr.move);

  tm->state = instr.next_state;
}

void dump_tm(TuringMachine *tm) 
{
  printf("Head dir: %i\n", tm->head_dir);
  printf("Head pos: %llu\n", tm->head_pos);
  printf("State: %c\n", tm->state);
}

