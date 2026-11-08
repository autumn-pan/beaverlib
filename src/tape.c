#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tape.h"
#include "tm.h"

uint64_t iabs(int64_t n)
{
  int64_t mask = n >> 63;

  return (n ^ mask) - mask;
}

Tape *tape_new()
{
  Tape *tape = malloc(sizeof(Tape));
  if (!tape)
  {
    return NULL;
  }

  // Since we use calloc, the tapes already read 0 on every cell
  tape->left_tape = calloc(HALF_TAPE_SIZE, sizeof(uint64_t));
  tape->right_tape = calloc(HALF_TAPE_SIZE, sizeof(uint64_t));

  tape->left_tape_size = HALF_TAPE_SIZE;
  tape->right_tape_size = HALF_TAPE_SIZE;

  tape->head_dir = 1;
  tape->head_pos = 0;

  return tape;
}

// append an additional uint64_t to a half-tape
static void extend_half_tape(uint64_t **tape, size_t *tape_size)
{
  size_t new_size = *tape_size + 1;
  uint64_t *new_tape = realloc(*tape, new_size * sizeof(uint64_t));

  if (!new_tape)
    return;

  // zero out the new tape
  memset(new_tape + *tape_size, 0, sizeof(uint64_t));
  *tape = new_tape;
  *tape_size = new_size;
}

// append an additional uint64_t to the left half-tape
void extend_left_tape(Tape *tape)
{
  extend_half_tape(&tape->left_tape, &tape->left_tape_size);
}

// append an additional uint64_t to a right half-tape
void extend_right_tape(Tape *tape)
{
  extend_half_tape(&tape->right_tape, &tape->right_tape_size);
}

bool read(Tape *tape)
{
  if (tape->head_dir > 0)
  {
    size_t word = (size_t)tape->head_pos / 64;
    size_t bit = (size_t)tape->head_pos % 64;
    return (tape->right_tape[word] >> bit) & 1ULL;
  }

  size_t word = (size_t)((tape->head_pos)) / 64;
  size_t bit = (size_t)((tape->head_pos)) % 64;
  return (tape->left_tape[word] >> bit) & 1ULL;
}

void write(Tape *tape, bool value)
{
  if (tape->head_dir == 1)
  {
    size_t word = (size_t)tape->head_pos / 64;
    size_t bit = (size_t)tape->head_pos % 64;

    if (value)
      tape->right_tape[word] |= (1ULL << bit);
    else
      tape->right_tape[word] &= ~(1ULL << bit);
      
    return;
  }

  size_t word = (size_t)((tape->head_pos)) / 64;
  size_t bit = (size_t)((tape->head_pos)) % 64;

  if (value)
    tape->left_tape[word] |= (1ULL << bit);
  else
    tape->left_tape[word] &= ~(1ULL << bit);
}

void move(Tape *tape, bool dir)
{
  if(tape->head_dir)
  {
    // Right tape case
    if(dir)
    {
      tape->head_pos++;
      if((uint64_t)(tape->head_pos/64) > tape->right_tape_size)
        extend_right_tape(tape);
    }
    else
    {
      if(tape->head_pos == 0)
        tape->head_dir = 0;
      else
        tape->head_pos--;
    }
  }
  else
  {
    // Left tape case
    if(!dir)
    {
      tape->head_pos++;
      if((uint64_t)(tape->head_pos/64) > tape->left_tape_size)
        extend_left_tape(tape);
    }
    else
    {
      if(tape->head_pos == 0)
        tape->head_dir = 1;
      else
        tape->head_pos--;
    }
  }
}

Tape *clone_tape(const Tape *ref) 
{
  // Make the new tape
  Tape *new = tape_new();

  // Copy basic metadata
  new->head_dir = ref->head_dir;
  new->head_pos = ref->head_pos;

  // Copy the tape contents

  new->left_tape_size = ref->left_tape_size;
  new->right_tape_size = ref->right_tape_size;

  new->left_tape = calloc(new->left_tape_size, sizeof(uint64_t));
  new->right_tape = calloc(new->right_tape_size, sizeof(uint64_t));

  // We need to be careful on how we copy the raw tape values
  // because they are pointers
  for(int i = 0; i < new->left_tape_size; i++)
    new->left_tape[i] = ref->left_tape[i];

  for(int i = 0; i < new->left_tape_size; i++)
    new->right_tape[i] = ref->right_tape[i];

  // Return the new tape
  return new;
}

void dump_tape(const Tape *tape)
{
  for(int i = 0; i < tape->left_tape_size; i++)
  {
    if(tape->left_tape[i] != 0)
      printf("%llu ", tape->left_tape[i]);
  }
  printf("\n\n");
  for(int i = 0; i < tape->right_tape_size; i++)
  {
    if(tape->right_tape[i] != 0)
      printf("%llu ", tape->right_tape[i]);
  }
}