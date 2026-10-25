#ifndef TM_H
#define TM_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define HALF_TAPE_SIZE 1

typedef struct
{
  bool write;
  bool move;
  char next_state;
} Instruction;

typedef struct 
{
  // The tape is split into two halves at the starting position
  uint64_t *left_tape; 
  uint64_t *right_tape; 

  // Number of uint64_t's in each half-tape
  size_t left_tape_size;
  size_t right_tape_size;

  int64_t head_pos;
  bool head_dir; // Whether or not the head is on the right half-tape

  char state;
  Instruction **trans_table; // Transition table
} TuringMachine;

#endif