#ifndef UNIQUE_SET_H
#define UNIQUE_SET_H

#include "hashmap.h"

typedef struct
{
  hashmap map;
} unique_set;

static unique_set
init_set()
{
  unique_set s;
  s.map = init_hashmap(100);
  return(s);
}

static bool
insert(unique_set *s, int32_t key)
{
  char key_string[4];
  memcpy(key_string, (char *) &key, 4);
  return(set(&(s->map), key_string, 4, 1));
}

static bool
has(unique_set *s, int32_t key)
{
  char key_string[4];
  memcpy(key_string, (char *) &key, 4);
  return(contains(&(s->map), key_string, 4));
}

static bool
add(unique_set *s, vector *vec, bool do_filter, int32_t filter)
{
  bool updated = false;
  for (uint32_t i = 0; i < vec->size; ++i)
  {
    if (!do_filter || vec->data[i] != filter)
    {
      bool updated_now = insert(s, vec->data[i]);
      updated = updated || updated_now;
    }
  }

  return(updated);
}

static vector
items(unique_set *s)
{
  vector res = init_vector(s->map.key_count);

  for (uint32_t i = 0; i < s->map.bucket_count; ++i)
  {
    keyval *kv = s->map.buckets[i];
    while (kv != 0)
    {
      int32_t key;
      memcpy(&key, kv->key, 4);
      push_back(&res, key);
      kv = kv->next;
    }
  }

  return(res);
}

static void
free_set(unique_set *s)
{
  free_hashmap(&(s->map));
}

#endif