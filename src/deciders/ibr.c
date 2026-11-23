#include "deciders/ibr.h"
#include "tm.h"
#include "deciders/forward_sim.h"

// The mapping of configs to tms is not bijective because knowledge must be lost
TuringMachine *config2tm(IBR *ibr, HaltConfig *config, size_t runtime)
{
  Tape *tape = clone_tape(config->tape);

  TuringMachine *tm = tm_new(ibr->instructions);
  tm->head_dir = tape->head_dir;
  tm->head_pos = tape->head_pos;
  tm->left_tape = tape->left_tape;
  tm->left_tape_size = tape->left_tape_size;
  tm->right_tape = tape->right_tape_size;
  tm->state = config->state;
  
  return tm;
}

void run_loop_detection()
{

}

void run(Instruction *instructions) 
{
  
}

