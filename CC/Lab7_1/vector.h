#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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


// NOTE(aolo2): for lab 7.2
/*static void
append(vector *vec, vector *append_me)
{
  uint64_t append_size = append_me->size; // NOTE(aolo2): saved in case vec == append_me
  for (uint64_t i = 0; i < append_size; ++i)
  {
    push_back(vec, append_me->data[i]);
  }
}*/

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
  printf(" ]\n");
}

#endif