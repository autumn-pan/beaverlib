#ifndef BACKWARD_H
#define BACKWARD_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct halting_config
{
  struct halting_config **children;
  size_t num_children;

  uint64_t *right_tape;
  uint64_t *left_tape;

  size_t num_right_segments;
  size_t num_left_segments;

  uint8_t state;
  int64_t head_pos;

  int64_t known_min;
  int64_t known_max;
} HaltingConfiguration;

typedef struct 
{
  uint8_t state;
  uint8_t new_state;
  bool read;
  bool write;
} InverseInstruction;

typedef struct 
{
  // Predecessors that write 0
  InverseInstruction *pred0;
  // Predecessors that write 1
  InverseInstruction *pred1;

  // Metadata for purpose of iteration
  size_t num_pred0;
  size_t num_pred1;
} InverseEntry;


#endif