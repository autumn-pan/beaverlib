#include "backward.h"
#include "tm.h"
#include "parser.h"

#include <stdint.h>
#include <stdbool.h>

HaltingConfiguration* hc_new(uint8_t state, int64_t pos, int64_t min, int64_t max)
{
  HaltingConfiguration* hc = malloc(sizeof(HaltingConfiguration));
  if(!hc)
    return NULL;
  
  hc->num_children = 0;
  hc->left_tape = calloc(1, sizeof(uint64_t));
  hc->right_tape = calloc(1, sizeof(uint64_t));

  hc->num_left_segments = 1;
  hc->num_right_segments = 1;

  hc->state = state;

  hc->known_min = min;
  hc->known_max = max;
  hc->head_pos = pos;

  return hc;
}

void hc_child_append(HaltingConfiguration *parent, HaltingConfiguration *child)
{
  parent->num_children++;
  // Maybe it needs to be an array of ptrs?
  parent->children = realloc(parent->children, 
                             parent->num_children*sizeof(HaltingConfiguration));
  parent->children[parent->num_children] = child;
}

void run_br(TuringMachine *tm)
{
  if(!tm)
    return;

  // First, we need to fetch the halting state and get its coordinates in the
  // transition table.
  // Then, we use that to create the root node.  
  return;
}


/* Optimized inverse lookup table. I want to get the brute force method working first.
InverseInstruction inverse_instruction_new(Instruction instr, uint8_t state, bool read)
{
  InverseInstruction inverse;
  inverse.write = instr.write;
  inverse.read = read;
  inverse.state = state;
  inverse.new_state = instr.next_state;

}

InverseEntry *gen_inverse_entry(Instruction **table, uint8_t state)
{
  InverseEntry *entry = malloc(sizeof(InverseEntry));
  if(!entry)
    return;

  entry->num_pred0 = 0;
  entry->num_pred1 = 0;

  for(int i = 0; i < 2; i++)
  {
    for(int j = 0; j < 6; j++)
    {
      if(table[i][j].next_state == state)
      {
        if(table[i][j].write == 0)
        {
          entry->num_pred0++;
          entry->pred0 = realloc(entry->pred0, 
                                     entry->num_pred0*sizeof(InverseInstruction));
          entry->pred0[entry->num_pred0] = 
        }
        else
        {
          entry->num_pred1++;
          entry->pred1 = realloc(entry->pred1, 
                                     entry->num_pred1*sizeof(InverseInstruction));
          entry->pred1[entry->num_pred1] = 
        }

        
      }
    }
  }
}

InverseEntry **gen_inverse_table(TuringMachine *tm)
{
  if(!tm)
    return NULL;

}
*/

asdfghjklasdf