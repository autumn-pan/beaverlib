#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tm.h"

TuringMachine *tm_new(Instruction **trans_table)
{
  TuringMachine *tm = malloc(sizeof(TuringMachine));
  if (!tm)
  {
    return NULL;
  }

  // Since we use calloc, the tapes already read 0 on every cell
  tm->left_tape = calloc(HALF_TAPE_SIZE, sizeof(uint64_t));
  tm->right_tape = calloc(HALF_TAPE_SIZE, sizeof(uint64_t));

  tm->left_tape_size = HALF_TAPE_SIZE;
  tm->right_tape_size = HALF_TAPE_SIZE;

  tm->head_dir = 1;
  tm->head_pos = 0;

  tm->state = 'A';

  tm->trans_table = trans_table;

  return tm;
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
void extend_left_tape(TuringMachine *tm)
{
  extend_half_tape(&tm->left_tape, &tm->left_tape_size);
}

// append an additional uint64_t to a right half-tape
void extend_right_tape(TuringMachine *tm)
{
  extend_half_tape(&tm->right_tape, &tm->right_tape_size);
}

bool read(TuringMachine *tm)
{
  if (tm->head_pos >= 0)
  {
    size_t word = (size_t)tm->head_pos / 64;
    size_t bit = (size_t)tm->head_pos % 64;
    return (tm->right_tape[word] >> bit) & 1U;
  }

  size_t word = (size_t)(-(tm->head_pos + 1)) / 64;
  size_t bit = (size_t)(-(tm->head_pos + 1)) % 64;
  return (tm->left_tape[word] >> bit) & 1U;
}

void write(TuringMachine *tm, bool value)
{
  if (tm->head_pos >= 0)
  {
    size_t word = (size_t)tm->head_pos / 64;
    size_t bit = (size_t)tm->head_pos % 64;

    if (value)
      tm->right_tape[word] |= (1ULL << bit);
    else
      tm->right_tape[word] &= ~(1ULL << bit);
    return;
  }

  size_t word = (size_t)(-(tm->head_pos + 1)) / 64;
  size_t bit = (size_t)(-(tm->head_pos + 1)) % 64;

  if (value)
    tm->left_tape[word] |= (1ULL << bit);
  else
    tm->left_tape[word] &= ~(1ULL << bit);
}

void move(TuringMachine *tm, bool dir)
{
  if (dir)
    tm->head_pos++;
  else
    tm->head_pos--;

  if (tm->head_pos >= 0)
  {
    tm->head_dir = 1;
    if ((size_t)tm->head_pos / 64 >= tm->right_tape_size)
      extend_right_tape(tm);
  }
  else
  {
    tm->head_dir = 0;
    if ((size_t)(-(tm->head_pos + 1)) / 64 >= tm->left_tape_size)
      extend_left_tape(tm);
  }
}

int tm_step(TuringMachine *tm)
{
  if (!tm || tm->state == '-')
  {
    printf("Halted!");
    return 0;
  }

  if (tm->head_pos >= 0)
  {
    if ((size_t)tm->head_pos / 64 >= tm->right_tape_size)
      extend_right_tape(tm);
  }
  else
  {
    if ((size_t)(-(tm->head_pos + 1)) / 64 >= tm->left_tape_size)
      extend_left_tape(tm);
  }

  bool read_value = read(tm);

  if (tm->state < 'A' || tm->state > 'Z')
    return 2;

  Instruction instr = tm->trans_table[read_value][tm->state - 'A'];

  write(tm, instr.write);
  read_value = read(tm);

  move(tm, instr.move);

  tm->state = instr.next_state;

  return 1;
}

void dump_tm(TuringMachine *tm)
{
  printf("Head dir: %i\n", tm->head_dir);
  printf("Head pos: %llu\n", tm->head_pos);
  printf("State: %c\n", tm->state);
}

void save_tm(TuringMachine *tm)
{
  printf("rts: %llu", tm->right_tape_size);
  printf("lts: %llu", tm->left_tape_size);

  for(int i = 0; i < tm->left_tape_size; i++)
    printf("%llu ", tm->left_tape[i]);

  printf("| ");

  for(int i = 0; i < tm->right_tape_size; i++)
    printf("%llu ", tm->right_tape[i]);

  printf("| %llu", tm->head_pos);
  printf("| %llu", tm->head_dir);
  printf("| %c", tm->state);
}