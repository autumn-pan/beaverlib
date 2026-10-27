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

#endif