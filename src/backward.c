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

// Create a blank tape
Tape *blank_tape_new()
{
  Tape *tape = malloc(sizeof(Tape));
  if(!tape)
    return NULL;

  tape->tape = calloc(1, sizeof(uint32_t));
  if(!tape->tape)
    return NULL;

  tape->num_tape = 1;
  tape->head_pos = 16;
}

// Clone the data from one tape to another
Tape *tape_copy(const Tape *original)
{
  Tape *new = blank_tape_new();
  if(!new)
    return NULL;

  new->head_pos = original->head_pos;
  new->num_tape = original->num_tape;

  // The tape is an array of integers, so we will need to iterate through that to copy data
  new->tape = realloc(new->tape, new->num_tape*sizeof(uint32_t));
  for(int i = 0; i < new->num_tape; i++)
  {
    new->tape[i] = original->tape[i];
  }

  return new;
}

void tape_write(Tape *tape, size_t pos, bool value)
{
  if(pos < 0 || pos > tape->num_tape*32 - 1)
    return;

  size_t fi = pos/32;
  size_t si = pos%32;
  uint32_t tape_seg = tape->tape[fi];

  // Write the value
  if(value)
    tape_seg |= 1 << si;
  else
    tape_seg &= ~(1 << si);
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