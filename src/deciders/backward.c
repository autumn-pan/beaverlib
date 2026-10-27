#include "backward.h"
#include "tm.h"
#include "parser.h"


// Not optimal but proportionally irrelevant
InverseEntry *inverse_entry_gen(TuringMachine *tm, uint8_t state)
{
  InverseEntry *entry = malloc(sizeof(InverseEntry));
  if(!entry)
    return NULL;

  // Transition table for the tm
  Instruction **table = parse_tm(tm);

  // List of every instruction that is a predecessor to the state
  Instruction *candidate_list;
  entry->num_pred = 0;

  // Generate candidate_list
  for(int i = 0; i < 2; i++)
  {
    for(int j = 0; j < 6; j++)
    {
      Instruction candidate = table[i][j];

      if(candidate.next_state == state)
      {
        entry->num_pred++;
        candidate_list = realloc(candidate_list, entry->num_pred*sizeof(Instruction));
        if(!candidate_list)
          return NULL;

        candidate_list[entry->num_pred - 1] = candidate;
      }
    }
  }

  // Process the candidate list

  entry->direction = calloc(entry->num_pred, sizeof(bool));
  entry->new_symbol = calloc(entry->num_pred, sizeof(bool));
  entry->req_symbol = calloc(entry->num_pred, sizeof(bool));
  entry->state = calloc(entry->num_pred, sizeof(uint8_t));

  // Check if there was an allocation error
  if(!entry->direction || !entry->new_symbol || !entry->req_symbol || !entry->state)
    return NULL;

  for(int i = 0; i < entry->num_pred; i++)
  {
    entry->direction[i] = !candidate_list[i].move;
    entry->new_symbol[i] = candidate_list[i].req_symbol;
    entry->req_symbol[i] = candidate_list[i].write;
    entry->state[i] = candidate_list[i].req_state;
  }

  return entry;
}

HaltConfig *halt_config_new(uint8_t state, size_t pos, Tape *tape)
{
  HaltConfig *config = malloc(sizeof(HaltConfig));
  if(!config)
    return NULL;

  config->num_children = NULL;
  config->state = state;
  config->tape = tape;
}

DeciderBR *br_decider_gen(TuringMachine *tm)
{
  DeciderBR *decider = malloc(sizeof(DeciderBR));
  if(!decider)
    return NULL;

  for(int i = 0; i < 6; i++)
  {
    decider->inverse_table[i] = inverse_entry_gen(tm, i);
  }
}