#ifndef TREE_H
#define TREE_H

#include "vector.h"
#include "lexem.h"

const uint32_t INITIAL_FOREST_SIZE = 50;
const uint32_t INITIAL_CHILDREN_CAP = 5;

typedef struct
{
  uint32_t idx;
  bool leaf;
} tree;

typedef struct
{
  tree *nodes;
  vector *children;
  char **names;
  uint32_t tree_capacity;
  uint32_t tree_count;
  uint32_t tree_head;
  bool error;
} forest;


static tree
init_tree(bool is_leaf)
{
  tree t;

  t.leaf = is_leaf;

  return(t);
}

static forest
init_forest()
{
  forest f;

  f.nodes = (tree *) malloc(sizeof(tree) * INITIAL_FOREST_SIZE);
  assert(f.nodes != 0);
  f.children = (vector *) malloc(sizeof(vector) * INITIAL_FOREST_SIZE);
  assert(f.children != 0);
  f.names = (char **) malloc(sizeof(char *) * INITIAL_FOREST_SIZE);
  assert(f.names != 0);

  f.tree_capacity = INITIAL_FOREST_SIZE;
  f.tree_head = 0;
  f.tree_count = 0;

  return(f);
}

static uint32_t
_add_node(forest *f, tree node, char *name, uint32_t name_len)
{
  if (f->tree_count >= f->tree_capacity)
  {
    f->nodes = realloc(f->nodes, sizeof(tree) * f->tree_capacity * 2);
    assert(f->nodes != 0);
    f->children = realloc(f->children, sizeof(vector) * f->tree_capacity * 2);
    assert(f->children != 0);
    f->names = realloc(f->names, sizeof(char *) * f->tree_capacity * 2);
    assert(f->names != 0);
    f->tree_capacity *= 2;
  }

  node.idx = f->tree_head;
  f->nodes[f->tree_head] = node;
  f->names[f->tree_head] = (char *) calloc(name_len + 1, sizeof(char)); // NOTE(aolo2): null-terminator
  f->children[f->tree_head] = init_vector(INITIAL_CHILDREN_CAP);
  assert(f->names[f->tree_head] != 0);
  memcpy(f->names[f->tree_head], name, name_len);

  ++(f->tree_head);
  ++(f->tree_count);

  return(node.idx);
}

static uint32_t
add_node(forest *f, tree node, char *name, uint32_t name_len, int64_t parent)
{
  uint32_t idx = _add_node(f, node, name, name_len);

  if (idx != 0)
  {
    assert(parent < f->tree_count);
    f->nodes[parent].leaf = false;
    push_back(&(f->children[parent]), idx);
  }

  return(idx);
}

/*static uint32_t
add_leaf(forest *f, parse_pair pp)
{
  return(add_node(
    f,
    init_tree(true),
    t_string((T) (-1 * pp.s32[SYMBOL_NUM])), pp.s32[PARENT_IDX])
  );
}*/

static void
print_forest_rec(uint32_t idx, uint32_t depth, vector last, forest *f)
{
  for (uint32_t i = 1; i < depth; ++i)
  {
    printf("%s", (last.data[i] ? "    " : "│   "));
  }

  if (depth > 0)
  {
    printf("%s%s\n", (peek_back(&last) ? "└───" : "├───"), f->names[idx]);
  }
  else
  {
    printf("%s\n", f->names[idx]);
  }

  uint64_t children_size = f->children[idx].size;
  for (int32_t i = 0; i < children_size; ++i)
  {
    vector last_copy = vector_copy(&last);
    push_back(&last_copy, (i == children_size - 1 ? 1 : 0));
    print_forest_rec(f->children[idx].data[i], depth + 1, last_copy, f);
    free_vector(&last_copy);
  }
}

static void
print_forest(forest *f)
{
  if (f->tree_count > 0)
  {
    vector last = init_vector(INITIAL_CHILDREN_CAP);
    push_back(&last, 1);
    print_forest_rec(0, 0, last, f);
    free_vector(&last);
  }
}

static void
free_forest(forest *f)
{
  for (uint32_t i = 0; i < f->tree_count; ++i)
  {
    free(f->names[i]);
    free_vector(&(f->children[i]));
  }

  free(f->children);
  free(f->nodes);
  free(f->names);
}

#endif