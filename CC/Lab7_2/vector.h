#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

const uint32_t VECTOR_SIZE_MULTIPLIER = 2;

typedef struct
{
  int64_t *data;
  uint64_t size;
  uint64_t head;
  uint64_t capacity;
} vector;

typedef union
{
  uint64_t u64;
  int32_t s32[2];
} parse_pair;

enum
{
  SYMBOL_NUM,
  PARENT_IDX
};

static vector
init_vector(uint64_t capacity)
{
  vector vec;

  vec.data = (int64_t *) malloc(sizeof(int64_t) * capacity);
  vec.size = 0;
  vec.head = 0;
  vec.capacity = capacity;
  assert(vec.data != 0);

  return(vec);
}

static void
push_back(vector *vec, int64_t item)
{
  if (vec->size >= vec->capacity)
  {
    vec->data = (int64_t *) realloc(vec->data, sizeof(int64_t) * vec->capacity * VECTOR_SIZE_MULTIPLIER);
    vec->capacity *= VECTOR_SIZE_MULTIPLIER;
    assert(vec->data != 0);
  }

  vec->data[vec->head++] = item;
  ++(vec->size);
}

static vector
vector_copy(vector *vec)
{
  vector copy = init_vector(vec->size);

  memcpy(copy.data, vec->data, (vec->size) * sizeof(int64_t));
  copy.size = vec->size;
  copy.head = vec->head;

  return(copy);
}


static void
append(vector *vec, vector *append_me)
{
  while (vec->capacity < vec->size + append_me->size)
  {
    vec->data = (int64_t *) realloc(vec->data, vec->capacity * sizeof(int64_t) * VECTOR_SIZE_MULTIPLIER);
    vec->capacity *= VECTOR_SIZE_MULTIPLIER;
    assert(vec->data != 0);
  }

  memmove(&(vec->data[vec->head]), append_me->data, append_me->size * sizeof(int64_t));
  uint32_t old_size = append_me->size; // NOTE(aolo2): save, because append_me can be me :)

  vec->size += old_size;
  vec->head += old_size;

  assert(vec->data != 0);
}

static int64_t
peek_back(vector *vec)
{
  assert(vec->size > 0);
  return(vec->data[vec->head - 1]);
}

static void
pop_back(vector *vec)
{
  assert(vec->size > 0);
  --(vec->size);
  --(vec->head);
}

static inline void
free_vector(vector *vec)
{
  if (vec->data)
  {
    free(vec->data);
    vec->size = 0;
    vec->capacity = 0;
    vec->data = 0;
  }
}

static inline bool
is_empty(vector *vec)
{
  return(vec->size == 0);
}

// NOTE(aolo2): for debugging
static void
print_vector(vector *vec)
{
  printf("[");
  for (uint64_t i = 0; i < vec->size; ++i)
  {
    printf(" %ld", vec->data[i]);
  }
  printf(" ]");
}

#endif