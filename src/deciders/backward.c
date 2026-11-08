#include "deciders/backward.h"
#include "tm.h"
#include "tape.h"
#include "parser.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Not optimal but proportionally irrelevant
InverseEntry *inverse_entry_gen(TuringMachine *tm, uint8_t state)
{
  InverseEntry *entry = malloc(sizeof(InverseEntry));
  if(!entry)
    return NULL;

  // Transition table for the tm
  Instruction **table = tm->trans_table;

  // List of every instruction that is a predecessor to the state
  Instruction *candidate_list = NULL;
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
        {
          printf("Error");
          fflush(stdout);
          return NULL;
        }

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

HaltConfig *halt_config_new(uint8_t state, Tape *tape)
{
  HaltConfig *config = malloc(sizeof(HaltConfig));
  if(!config)
    return NULL;

  config->num_children = 0;
  config->state = state;
  config->tape = tape;

  return config;
}

void append_config(HaltConfig *parent, HaltConfig *child)
{
  parent->num_children++;
  parent->children = realloc(parent->children, parent->num_children);

  parent->children[parent->num_children - 1] = child;
}

DeciderBR *br_decider_gen(TuringMachine *tm)
{
  DeciderBR *decider = malloc(sizeof(DeciderBR));
  if(!decider)
    return NULL;

  Instruction **table = tm->trans_table;
  decider->inverse_table = calloc(6, sizeof(InverseEntry));
  for(int i = 0; i < 6; i++)
  {
    decider->inverse_table[i] = inverse_entry_gen(tm, i);
  }
  
  // Find the halt state
  uint8_t halt_i;
  uint8_t halt_j;

  for(int i = 0; i < 2; i++)
  {
    for(int j = 0; j < 6; j++)
    {
      if(table[i][j].next_state == '-' - 'A');
      {
        halt_i = i;
        halt_j = j;
      }
    }
  }

  Tape *initial_tape = tape_new();
  write2(initial_tape, halt_i);
  decider->root = halt_config_new(halt_j, initial_tape);
  decider->root->known_inf = 0;
  decider->root->known_sup = 1;
  printf("Halt i-index: %i\n", halt_i);
  printf("Halt j-index: %i\n", halt_j);

  return decider;
}


void dump_br_table(DeciderBR *decider)
{
  printf("Dumping BR Table: \n\n");
  for(int i = 0; i < 6; i++)
  {
    InverseEntry *entry = decider->inverse_table[i];

    printf("%c: ", i+'A');
    for(int j = 0; j < entry->num_pred; j++)
    {
      printf("%i ", entry->req_symbol[j]);
      printf("%i%i%c  ", entry->direction[j], entry->new_symbol[j], entry->state[j] + 'A');
    }

    printf("\n");
  }
}

void br_step(DeciderBR *decider, HaltConfig *config)
{
  // Fetch the relevant inverse table entry
  InverseEntry *entry = decider->inverse_table[config->state];

  for(int i = 0; i < entry->num_pred; i++)
  {
    int8_t dir = (!entry->direction[i] << 1) - 1;
    uint8_t req_state = entry->state[i];
    bool req_symbol = entry->req_symbol;

    bool new_dir = config->tape->head_dir;
    bool new_pos = config->tape->head_pos;

    if((int64_t)config->tape->head_pos + dir < 0)
    {
      new_dir = !config->tape->head_dir;
      new_pos = 0;
    }
    else
    {
      new_pos = new_pos + dir;
    }

    printf("New dir: %i, New pos: %i\n", new_dir, new_pos);
    printf("Read: %i\n", read_to_pos(config->tape, new_dir, new_pos));
    printf("Write value: %i\n", entry->req_symbol[i]);
    if(entry->req_symbol[i] != read_to_pos(config->tape, new_dir, new_pos))
      if((new_dir && new_pos < config->known_sup) || (!new_dir && new_pos < config->known_inf))
        continue;

    HaltConfig *child;

    Tape *new_tape = clone_tape(config->tape);
    new_tape->head_dir = new_dir;
    new_tape->head_pos = new_pos;
    write2(new_tape, entry->req_symbol[i]);
    child = halt_config_new(entry->state[i], new_tape);

    if(new_dir && new_pos > config->known_sup)
      child->known_sup++;
    else if(!new_dir && new_pos > config->known_inf)
      child->known_inf++;
    
    append_config(config, child);
  }
}