#ifndef BACKWARD_H
#define BACKWARD_H

#include <stdlib.h>
#include <stdint.h>

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

#endif