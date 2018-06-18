#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* <string, int32> hashmap */

typedef struct keyval
{
  uint32_t len;
  int32_t value;
  struct keyval *next;
  char *key;
} keyval;

typedef struct
{
  uint32_t bucket_count;
  uint32_t key_count;
  keyval **buckets;
} hashmap;

static keyval*
init_keyval(char *key, uint32_t len, int32_t value)
{
  keyval *res = (keyval *) malloc(sizeof(keyval));

  res->key = (char *) calloc((len + 1), sizeof(char));
  memcpy(res->key, key, len * sizeof(char));

  res->len = len;
  res->value = value;
  res->next = 0;

  assert(res->key != 0);

  return(res);
}

static hashmap
init_hashmap(uint32_t bucket_count)
{
  hashmap map;
  map.buckets = (keyval **) calloc(bucket_count, sizeof(keyval *)); // NOTE(aolo2): calloc -> all buckets are null ptrs!
  map.bucket_count = bucket_count;
  map.key_count = 0;

  assert(map.buckets != 0);

  return(map);
}

static uint32_t
hash(char *str, uint32_t len)
{
  uint64_t hash = 5381;
  int32_t c;

  for (uint32_t i = 0; i < len; ++i)
  {
    c = str[i];
    hash = ((hash << 5) + hash) + c;
  }

  return(hash);
}

static inline bool
str_equals(char *s1, uint32_t l1, char *s2, uint32_t l2)
{
  if (l2 > l1)
  {
    l1 = l2;
  }
  return(strncmp(s1, s2, l1) == 0 && strncmp(s2, s1, l1) == 0);
}

static keyval*
find(hashmap *map, char *key, uint32_t len)
{
  uint32_t hash_code = hash(key, len);
  uint32_t bucket_idx = hash_code % map->bucket_count;

  if (map->buckets[bucket_idx] == 0)
  {
    return(0);
  }

  keyval *pair = map->buckets[bucket_idx];
  while (pair != 0)
  {
    if (str_equals(pair->key, pair->len, key, len))
    {
      return(pair);
    }
    pair = pair->next;
  }

  return(0);
}


static bool
contains(hashmap *map, char *key, uint32_t len)
{
  keyval *res = find(map, key, len);
  return(res != 0);
}

static int32_t
get(hashmap *map, char *key, uint32_t len)
{
  keyval *res = find(map, key, len);
  assert(res != 0); // TODO(aolo2): what to do?
  return(res->value);
}

static void
insert_keyval(hashmap *map, uint32_t bucket_idx, keyval *insert)
{
  if (map->buckets[bucket_idx] == 0)
  {
    map->buckets[bucket_idx] = insert;
  }
  else
  {
    keyval *pair = map->buckets[bucket_idx];
    while (pair->next != 0)
    {
      pair = pair->next;
    }
    pair->next = insert;
  }
}

static bool
set(hashmap *map, char *key, uint32_t len, int32_t value)
{
  uint32_t hash_code = hash(key, len);
  uint32_t bucket_idx = hash_code % map->bucket_count;
  bool updated = false;

  keyval *old = find(map, key, len);

  if (old != 0)
  {
    if (old->value != value)
    {
      old->value = value;
      updated = true;
    }
  }
  else
  {
    keyval *insert = init_keyval(key, len, value);
    insert_keyval(map, bucket_idx, insert);
    ++(map->key_count);
    updated = true;
  }

  return(updated);
}

static inline uint32_t
size(hashmap *map)
{
  return(map->key_count);
}

static void
print_hashmap(hashmap *map)
{
  printf("\n=== HASHMAP ===\n");
  for (uint32_t i = 0; i < map->bucket_count; ++i)
  {
    keyval *kv = map->buckets[i];
    while (kv != 0)
    {
      printf("(%d, %s)\n", kv->value, kv->key);
      kv = kv->next;
    }
  }
}

static void
free_keyval(keyval *kv)
{
  keyval *pair = kv;
  keyval *next;

  while (pair != 0)
  {
    next = pair->next;
    free(pair->key);
    free(pair);
    pair = next;
  }
}

static void
free_hashmap(hashmap *map)
{
  for (uint32_t i = 0; i < map->bucket_count; ++i)
  {
    if (map->buckets[i] != 0)
    {
      free_keyval(map->buckets[i]);
    }
  }

  free(map->buckets);
}

#endif