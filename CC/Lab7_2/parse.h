#ifndef PARSE_H
#define PARSE_H

#include "tree.h"
#include "vector.h"


vector TRANSITION[20][13];

const uint32_t CONTEXT_RANGE = 10;

static void init_table()
{
  for (uint32_t i = 0; i < 20; ++i)
  {
    for (uint32_t j = 0; j < 13; ++j)
    {
      TRANSITION[i][j] = init_vector(5);
    }
  }

  push_back(&(TRANSITION[GRAMMAR][K_NONTERMINAL]), (int64_t) (N_LIST));
  push_back(&(TRANSITION[GRAMMAR][K_NONTERMINAL]), (int64_t) (T_LIST));
  push_back(&(TRANSITION[GRAMMAR][K_NONTERMINAL]), (int64_t) (RULES));
  push_back(&(TRANSITION[GRAMMAR][K_NONTERMINAL]), (int64_t) (AXIOM));

  push_back(&(TRANSITION[N_LIST][K_NONTERMINAL]), -1 * (int64_t) (K_NONTERMINAL));
  push_back(&(TRANSITION[N_LIST][K_NONTERMINAL]), (int64_t) (N_LIST_ITEMS));
  push_back(&(TRANSITION[N_LIST][K_NONTERMINAL]), -1 * (int64_t) (SEMICOLON));

  push_back(&(TRANSITION[N_LIST_ITEMS][NT_NAME]), -1 * (int64_t) (NT_NAME));
  push_back(&(TRANSITION[N_LIST_ITEMS][NT_NAME]), (int64_t) (N_LIST_ITEMS_2));

  push_back(&(TRANSITION[N_LIST_ITEMS_2][COMMA]), -1 * (int64_t) (COMMA));
  push_back(&(TRANSITION[N_LIST_ITEMS_2][COMMA]), -1 * (int64_t) (NT_NAME));
  push_back(&(TRANSITION[N_LIST_ITEMS_2][COMMA]), (int64_t) (N_LIST_ITEMS_2));

  push_back(&(TRANSITION[T_LIST][K_TERMINAL]), -1 * (int64_t) (K_TERMINAL));
  push_back(&(TRANSITION[T_LIST][K_TERMINAL]), (int64_t) (T_LIST_ITEMS));
  push_back(&(TRANSITION[T_LIST][K_TERMINAL]), -1 * (int64_t) (SEMICOLON));

  push_back(&(TRANSITION[T_LIST_ITEMS][QUOTED_SYMBOL]), (int64_t) (TERMINAL));
  push_back(&(TRANSITION[T_LIST_ITEMS][QUOTED_SYMBOL]), (int64_t) (T_LIST_ITEMS_2));
  push_back(&(TRANSITION[T_LIST_ITEMS][LC_WORD]), (int64_t) (TERMINAL));
  push_back(&(TRANSITION[T_LIST_ITEMS][LC_WORD]), (int64_t) (T_LIST_ITEMS_2));

  push_back(&(TRANSITION[T_LIST_ITEMS_2][COMMA]), -1 * (int64_t) (COMMA));
  push_back(&(TRANSITION[T_LIST_ITEMS_2][COMMA]), (int64_t) (TERMINAL));
  push_back(&(TRANSITION[T_LIST_ITEMS_2][COMMA]), (int64_t) (T_LIST_ITEMS_2));

  push_back(&(TRANSITION[TERMINAL][QUOTED_SYMBOL]), -1 * (int64_t) (QUOTED_SYMBOL));
  push_back(&(TRANSITION[TERMINAL][LC_WORD]), -1 * (int64_t) (LC_WORD));

  push_back(&(TRANSITION[RULES][NT_NAME]), (int64_t) (RULE));
  push_back(&(TRANSITION[RULES][NT_NAME]), (int64_t) (RULES_2));

  push_back(&(TRANSITION[RULES_2][NT_NAME]), (int64_t) (RULE));
  push_back(&(TRANSITION[RULES_2][NT_NAME]), (int64_t) (RULES_2));

  push_back(&(TRANSITION[RULE][NT_NAME]), -1 * (int64_t) (NT_NAME));
  push_back(&(TRANSITION[RULE][NT_NAME]), -1 * (int64_t) (QUAD_EQUALS));
  push_back(&(TRANSITION[RULE][NT_NAME]), (int64_t) (RIGHT_PART));
  push_back(&(TRANSITION[RULE][NT_NAME]), -1 * (int64_t) (SEMICOLON));

  push_back(&(TRANSITION[RIGHT_PART][QUOTED_SYMBOL]), (int64_t) (ALTERNATIVE));
  push_back(&(TRANSITION[RIGHT_PART][QUOTED_SYMBOL]), (int64_t) (ALTERNATIVES));
  push_back(&(TRANSITION[RIGHT_PART][NT_NAME]), (int64_t) (ALTERNATIVE));
  push_back(&(TRANSITION[RIGHT_PART][NT_NAME]), (int64_t) (ALTERNATIVES));
  push_back(&(TRANSITION[RIGHT_PART][LC_WORD]), (int64_t) (ALTERNATIVE));
  push_back(&(TRANSITION[RIGHT_PART][LC_WORD]), (int64_t) (ALTERNATIVES));
  push_back(&(TRANSITION[RIGHT_PART][K_EPSILON]), -1 * (int64_t) (K_EPSILON));

  push_back(&(TRANSITION[ALTERNATIVES][OR]), -1 * (int64_t) (OR));
  push_back(&(TRANSITION[ALTERNATIVES][OR]), (int64_t) (ALTERNATIVE));
  push_back(&(TRANSITION[ALTERNATIVES][OR]), (int64_t) (ALTERNATIVES));

  push_back(&(TRANSITION[ALTERNATIVE][QUOTED_SYMBOL]), (int64_t) (TN));
  push_back(&(TRANSITION[ALTERNATIVE][QUOTED_SYMBOL]), (int64_t) (TN_LIST));
  push_back(&(TRANSITION[ALTERNATIVE][NT_NAME]), (int64_t) (TN));
  push_back(&(TRANSITION[ALTERNATIVE][NT_NAME]), (int64_t) (TN_LIST));
  push_back(&(TRANSITION[ALTERNATIVE][LC_WORD]), (int64_t) (TN));
  push_back(&(TRANSITION[ALTERNATIVE][LC_WORD]), (int64_t) (TN_LIST));
  push_back(&(TRANSITION[ALTERNATIVE][K_EPSILON]), (int64_t) (TN));
  push_back(&(TRANSITION[ALTERNATIVE][K_EPSILON]), (int64_t) (TN_LIST));

  push_back(&(TRANSITION[TN][QUOTED_SYMBOL]), (int64_t) (TERMINAL));
  push_back(&(TRANSITION[TN][LC_WORD]), (int64_t) (TERMINAL));
  push_back(&(TRANSITION[TN][NT_NAME]), -1 * (int64_t) (NT_NAME));
  push_back(&(TRANSITION[TN][K_EPSILON]), -1 * (int64_t) (K_EPSILON));

  push_back(&(TRANSITION[TN_LIST][QUOTED_SYMBOL]), (int64_t) (TN));
  push_back(&(TRANSITION[TN_LIST][QUOTED_SYMBOL]), (int64_t) (TN_LIST_2));
  push_back(&(TRANSITION[TN_LIST][LC_WORD]), (int64_t) (TN));
  push_back(&(TRANSITION[TN_LIST][LC_WORD]), (int64_t) (TN_LIST_2));
  push_back(&(TRANSITION[TN_LIST][NT_NAME]), (int64_t) (TN));
  push_back(&(TRANSITION[TN_LIST][NT_NAME]), (int64_t) (TN_LIST_2));
  push_back(&(TRANSITION[TN_LIST][K_EPSILON]), (int64_t) (TN));
  push_back(&(TRANSITION[TN_LIST][K_EPSILON]), (int64_t) (TN_LIST_2));

  push_back(&(TRANSITION[TN_LIST_2][QUOTED_SYMBOL]), (int64_t) (TN));
  push_back(&(TRANSITION[TN_LIST_2][QUOTED_SYMBOL]), (int64_t) (TN_LIST_2));
  push_back(&(TRANSITION[TN_LIST_2][LC_WORD]), (int64_t) (TN));
  push_back(&(TRANSITION[TN_LIST_2][LC_WORD]), (int64_t) (TN_LIST_2));
  push_back(&(TRANSITION[TN_LIST_2][NT_NAME]), (int64_t) (TN));
  push_back(&(TRANSITION[TN_LIST_2][NT_NAME]), (int64_t) (TN_LIST_2));
  push_back(&(TRANSITION[TN_LIST_2][K_EPSILON]), (int64_t) (TN));
  push_back(&(TRANSITION[TN_LIST_2][K_EPSILON]), (int64_t) (TN_LIST_2));

  push_back(&(TRANSITION[AXIOM][K_AXIOM]), -1 * (int64_t) (K_AXIOM));
  push_back(&(TRANSITION[AXIOM][K_AXIOM]), -1 * (int64_t) (NT_NAME));
  push_back(&(TRANSITION[AXIOM][K_AXIOM]), -1 * (int64_t) (SEMICOLON));
}

static void
free_table()
{
  for (uint32_t i = 0; i < 20; ++i)
  {
    for (uint32_t j = 0; j < 13; ++j)
    {
      free_vector(&(TRANSITION[i][j]));
    }
  }
}

static inline int64_t
create_s64(int32_t symbol_num, int32_t parent_idx)
{
  parse_pair pp;
  pp.s32[SYMBOL_NUM] = symbol_num;
  pp.s32[PARENT_IDX] = parent_idx;
  return((int64_t) pp.u64);
}

static inline parse_pair
read_pp(int64_t n)
{
  parse_pair pp;
  pp.u64 = (uint64_t) n;
  return(pp);
}

static uint32_t
count_digits(uint32_t n)
{
  return snprintf(NULL, 0, "%d", n);
}

static void
print_error(fragment *coords)
{
  printf("# Parser found an error at ");
  print_position(&(coords->starting));
  printf(": %.*s\n", coords->following.index - coords->starting.index, &(coords->starting.text[coords->starting.index]));
  for (uint32_t i = 0;
    i < (27 + count_digits(coords->starting.row) + 4 + count_digits(coords->starting.column) + 2);
    ++i)
  {
    printf(" ");
  }
  printf("^\n");
}

static forest
parse(scanner sc)
{
  forest result = init_forest();
  vector magazine = init_vector(10);

  // char *root_name = n_string(GRAMMAR);
  // add_root(&result, init_tree(false), root_name, strlen(root_name));
  push_back(&magazine, create_s64(-1 * (int32_t) EOP, -1)); // NOTE(aolo2): -1 idx will not be used
  push_back(&magazine, create_s64((int32_t) GRAMMAR, 0));

  bool read_next = false;
  token tok = next_token(&sc);
  int32_t a = -1 * (int32_t) tok.tag;

  parse_pair X = read_pp(peek_back(&magazine));
  int32_t symbol = X.s32[SYMBOL_NUM];

  uint32_t active_node = 0;
  result.error = false;

  while (symbol != EOP)
  {
    read_next = false;
    if (is_terminal(symbol))
    {
      if (symbol == a)
      {
        uint32_t tok_start = tok.coords.starting.index;
        uint32_t tok_follow = tok.coords.following.index;

        add_node(&result, init_tree(true),
          &(sc.program[tok_start]), tok_follow - tok_start, active_node); // NOTE(aolo2): ret value is ignored as leafes have no child nodes
        pop_back(&magazine);
        read_next = true;
      }
      else
      {
        print_error(&(tok.coords));
        result.error = true;
        break;
      }
    }
    else
    {
      assert(symbol >= 0 && a <= 0);

      vector rules = TRANSITION[symbol][(-1) * a];
      pop_back(&magazine);

      char *node_name = symbol_string(symbol);
      uint32_t node_idx = add_node(&result, init_tree(false),
        node_name, strlen(node_name), active_node);

      for (int32_t i = rules.size - 1; i >= 0; --i)
      {
        push_back(&magazine, create_s64(rules.data[i], node_idx));
      }
    }

    if (read_next)
    {
      tok = next_token(&sc);
      a = -1 * (int32_t) tok.tag;
    }

    X = read_pp(peek_back(&magazine));
    symbol = X.s32[SYMBOL_NUM];
    active_node = X.s32[PARENT_IDX];
  }

  free_vector(&magazine);

  return(result);
}

#endif