#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tape.h"
#include "tm.h"

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
  if (tape->head_pos >= 0)
  {
    size_t word = (size_t)tape->head_pos / 64;
    size_t bit = (size_t)tape->head_pos % 64;
    return (tape->right_tape[word] >> bit) & 1U;
  }

  size_t word = (size_t)(-(tape->head_pos + 1)) / 64;
  size_t bit = (size_t)(-(tape->head_pos + 1)) % 64;
  return (tape->left_tape[word] >> bit) & 1U;
}

void write(Tape *tape, bool value)
{
  if (tape->head_pos >= 0)
  {
    size_t word = (size_t)tape->head_pos / 64;
    size_t bit = (size_t)tape->head_pos % 64;

    if (value)
      tape->right_tape[word] |= (1ULL << bit);
    else
      tape->right_tape[word] &= ~(1ULL << bit);
    return;
  }

  size_t word = (size_t)(-(tape->head_pos + 1)) / 64;
  size_t bit = (size_t)(-(tape->head_pos + 1)) % 64;

  if (value)
    tape->left_tape[word] |= (1ULL << bit);
  else
    tape->left_tape[word] &= ~(1ULL << bit);
}

void move(Tape *tape, bool dir)
{
  if (dir)
    tape->head_pos++;
  else
    tape->head_pos--;

  if (tape->head_pos >= 0)
  {
    tape->head_dir = 1;
    if ((size_t)tape->head_pos / 64 >= tape->right_tape_size)
      extend_right_tape(tape);
  }
  else
  {
    tape->head_dir = 0;
    if ((size_t)(-(tape->head_pos + 1)) / 64 >= tape->left_tape_size)
      extend_left_tape(tape);
  }
}

int tape_upd_bounds(Tape *tape)
{
  if (tape->head_pos >= 0)
  {
    if ((size_t)tape->head_pos / 64 >= tape->right_tape_size)
      extend_right_tape(tape);
  }
  else
  {
    if ((size_t)(-(tape->head_pos + 1)) / 64 >= tape->left_tape_size)
      extend_left_tape(tape);
  }
}