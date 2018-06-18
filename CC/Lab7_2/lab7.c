#include "tree.h"
#include "vector.h"
#include "lexem.h"
#include "grammar.h"
#include "hashmap.h"
#include "parse.h"

static inline void
read_leaf(grammar *g, forest *pt, uint32_t root, bool terminal)
{
  char *name = pt->names[root];
  if (terminal)
  {
    add_terminal(g, name, strlen(name));
  }
  else
  {
    add_nonterminal(g, name, strlen(name));
  }
}

static inline void
read_axiom(grammar *g, forest *pt, uint32_t root)
{
  char *axiom_name = pt->names[pt->children[root].data[1]];
  set_axiom(g, get_id(g, axiom_name, strlen(axiom_name)));
}

static void
read_tn(grammar *g, forest *pt, uint32_t root, rule *r)
{
  root = pt->children[root].data[0];
  char *tn_name = pt->names[root];

  if (pt->children[root].size > 0)
  {
    tn_name = pt->names[pt->children[root].data[0]];
  }
  push_back(&(r->right), get_id(g, tn_name, strlen(tn_name)));
}

static void
read_tn_list(grammar *g, forest *pt, uint32_t root, rule *r)
{
  if (pt->children[root].size > 0)
  {
    read_tn(g, pt, pt->children[root].data[0], r);

    root = pt->children[root].data[1];
    while (pt->children[root].size > 0)
    {
      read_tn(g, pt, pt->children[root].data[0], r);
      root = pt->children[root].data[1];
    }
  }
}

static void
read_alternative(grammar *g, forest *pt, uint32_t root, char *left_part)
{
  rule *r = init_rule((uint32_t) get_id(g, left_part, strlen(left_part)));

  read_tn(g, pt, pt->children[root].data[0], r);
  read_tn_list(g, pt, pt->children[root].data[1], r);

  add_rule(g, r);
}

static void
read_alternatives(grammar *g, forest *pt, uint32_t root, char *left_part)
{
  if (pt->children[root].size > 0)
  {
    read_alternative(g, pt, pt->children[root].data[1], left_part);

    root = pt->children[root].data[2];
    while (pt->children[root].size > 0)
    {
      read_alternative(g, pt, pt->children[root].data[1], left_part);
      root = pt->children[root].data[2];
    }
  }
}

static inline void
read_right_part(grammar *g, forest *pt, uint32_t root, char *left_part)
{
  read_alternative(g, pt, pt->children[root].data[0], left_part);
  read_alternatives(g, pt, pt->children[root].data[1], left_part);
}

static inline void
read_rule(grammar *g, forest *pt, uint32_t root)
{
  read_right_part(g, pt,
    pt->children[root].data[2],
    pt->names[pt->children[root].data[0]]);
}

static void
read_rules_2(grammar *g, forest *pt, uint32_t root)
{
  if (pt->children[root].size > 0)
  {
    read_rule(g, pt, pt->children[root].data[0]);

    root = pt->children[root].data[1];
    while (pt->children[root].size > 0)
    {
      read_rule(g, pt, pt->children[root].data[0]);
      root = pt->children[root].data[1];
    }
  }
}

static inline void
read_rules(grammar *g, forest *pt, uint32_t root)
{
  read_rule(g, pt, pt->children[root].data[0]);
  read_rules_2(g, pt, pt->children[root].data[1]);
}

static void
read_t_list(grammar *g, forest *pt, uint32_t root)
{
  uint32_t t_list_items_idx = pt->children[root].data[1];
  uint32_t terminal_idx = pt->children[t_list_items_idx].data[0];

  read_leaf(g, pt, pt->children[terminal_idx].data[0], true);

  root = pt->children[t_list_items_idx].data[1];
  while (pt->children[root].size > 0)
  {
    terminal_idx = pt->children[root].data[1];
    read_leaf(g, pt, pt->children[terminal_idx].data[0], true);
    root = pt->children[root].data[2];
  }
}

static void
read_n_list(grammar *g, forest *pt, uint32_t root)
{
  uint32_t n_list_items_idx = pt->children[root].data[1];
  read_leaf(g, pt, pt->children[n_list_items_idx].data[0], false);

  root = pt->children[n_list_items_idx].data[1];
  while (pt->children[root].size > 0)
  {
    read_leaf(g, pt, pt->children[root].data[1], false);
    root = pt->children[root].data[2];
  }
}

static void
read_grammar(grammar *g, forest *pt, uint32_t root)
{
  read_n_list(g, pt, pt->children[root].data[0]);
  read_t_list(g, pt, pt->children[root].data[1]);
  read_rules(g, pt, pt->children[root].data[2]);
  read_axiom(g, pt, pt->children[root].data[3]);
}

static grammar
create_grammar(forest *pt)
{
  // NOTE(aolo2): initital setup
  grammar g = init_grammar();
  add_terminal(&g, "epsilon", 7);
  add_nonterminal(&g, "ERROR", 5);

  if (pt->tree_count > 0)
  {
    read_grammar(&g, pt, 0); // TODO(aolo2): handle semantic errors
  }

  // NOTE(aolo2): compute transition table based on added rules
  compute_transition(&g);

  return(g);
}

int32_t
main(int32_t argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: %s INPUT\n", argv[0]);
    return(1);
  }

  char *text = read_file(argv[1]);
  if (text == 0)
  {
    return(1);
  }

  printf("\n=== INPUT ===\n%s\n", text);

  init_table();

  forest parse_tree = parse(init_scanner(text, strlen(text)));

  if (!parse_tree.error)
  {
    // print_forest(&parse_tree);

    grammar g = create_grammar(&parse_tree);
    print_grammar(&g);

    free_grammar(&g);
  }

  free_forest(&parse_tree);
  free_table();
  free(text);

  return(0);
}