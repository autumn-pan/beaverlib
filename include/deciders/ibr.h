#ifndef IBR_H
#define IBR_H

#include "deciders/backward.h"
#include "tm.h"

typedef struct {
  HaltConfig *root;
  HaltConfig *terminal;
  HaltConfig *mask;

  size_t num_steps;
} PartialInvMac;

// In general, only use macros to describe an end state, don't attempt to use them as a shortcut
// Macros are expensive to use and are meant to prove recursive rules and invariants
typedef struct {
  HaltConfig *root;

  size_t num_leaves;
} InverseMacro;



typedef struct {
  Instruction *instructions;
  // Number of primary macros remains at 6
  InverseMacro *primary_macros;

  InverseMacro *secondarsy_macros;
  size_t num_sec_macros;
} IBR;

#endif