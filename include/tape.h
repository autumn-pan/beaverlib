#ifndef TAPE_H
#define TAPE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct 
{
  uint64_t *left_tape; 
  uint64_t *right_tape; 

  // Number of uint64_t's in each half-tape
  size_t left_tape_size;
  size_t right_tape_size;

  int64_t head_pos;
  bool head_dir; // Whether or not the head is on the right half-tape
} Tape;

Tape *tape_new();
void write2(Tape *tape, bool value);
void dump_tape2(const Tape *tape);
bool read2(Tape *tape);
bool read_to_pos(Tape *tape, bool head_dir, size_t head_pos);
void write_to_pos(Tape *tape, bool value, bool head_dir, size_t head_pos);
Tape *clone_tape(const Tape *ref);
void extend_left_tape2(Tape *tape);
void extend_right_tape2(Tape *tape);

#endif