#ifndef BR_H
#define BR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// TODO: split this into a segregated list for efficiency
typedef struct {
    // What state does the predecessor come from?
  uint8_t *state;

  // What direction does the predecessor need to be relative to the successor?
  bool *direction;

  // What symbol does the predecessor demand?
  // This is equivalent to the "write" parameter
  bool *req_symbol;

  // What symbol does the predecessor write?
  // This is equivalent to the symbolic index on the transition table
  bool *new_symbol;

  // Number of predecessors; length of each array
  size_t num_pred;
} InverseEntry;

typedef struct 
{
  uint32_t *tape;
  size_t num_tape;

  size_t head_pos;
} Tape;

typedef struct halt_config
{
  struct halt_config **children;
  size_t num_children;
  uint8_t state;

  Tape *tape;
} HaltConfig;

typedef struct
{
  HaltConfig *root;
  
  InverseEntry *inverse_table[6];
} DeciderBR;

#endif
