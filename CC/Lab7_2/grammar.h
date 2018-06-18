#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "vector.h"
#include "hashmap.h"
#include "unique_set.h"

const uint32_t INITIAL_RULE_COUNT = 5;
const uint32_t INITIAL_RULE_RHS_LEN = 4;
const uint32_t INITIAL_NAMES_CAPACITY = 10;

typedef struct rule
{
  uint32_t left;
  vector right;
  struct rule *next;
} rule;

typedef struct
{
  uint32_t axiom;
  uint32_t rule_capacity;
  uint32_t nonterm_count;
  int32_t term_count;
  hashmap symbols;
  rule **rules; // NOTE(aolo2): all rules for each non-terminal
  vector **transition;

  // NOTE(aolo2): needed for debug only
  uint32_t names_capacity;
  uint32_t names_count;
  vector term_names_idx;
  vector nonterm_names_idx;
  char **names;
} grammar;


// NOTE(aolo2): prototype because mutual recursion
static vector _first(grammar *g, int32_t id);
static vector _first_rec(grammar *g, int32_t id, rule *r);
static inline int32_t get_id(grammar *g, char *name, uint32_t name_len);

static grammar
init_grammar()
{
  grammar g;

  g.axiom = -1; // NOTE(aolo2): negative value means axiom hasn't been set
  g.symbols = init_hashmap(INITIAL_NAMES_CAPACITY);
  g.term_count = 0;
  g.nonterm_count = 0;
  g.rules = (rule **) malloc(sizeof(rule *) * INITIAL_RULE_COUNT);
  g.rule_capacity = INITIAL_RULE_COUNT;
  assert(g.rules != 0);

  for (uint32_t i = 0; i < g.rule_capacity; ++i)
  {
    g.rules[i] = 0;
  }

  g.names = (char **) calloc(INITIAL_NAMES_CAPACITY, sizeof(char **));
  g.names_count = 0;
  g.names_capacity = INITIAL_NAMES_CAPACITY;
  assert(g.names != 0);

  g.term_names_idx = init_vector(INITIAL_NAMES_CAPACITY);
  g.nonterm_names_idx = init_vector(INITIAL_NAMES_CAPACITY);

  return(g);
}

static rule*
init_rule(uint32_t left)
{
  rule *r = (rule *) malloc(sizeof(rule));

  r->left = left;
  r->right = init_vector(INITIAL_RULE_RHS_LEN);
  r->next = 0;

  return(r);
}

static inline void
set_axiom(grammar *g, int32_t id)
{
  assert(id > 0);
  g->axiom = id;
}

static inline bool
is_axiom(grammar *g, int32_t id)
{
  assert(g->axiom > 0);
  return(id == g->axiom);
}

static uint32_t
add_name(grammar *g, char* name, uint32_t name_len)
{
  if (g->names_count >= g->names_capacity)
  {
    g->names = (char **) realloc(g->names, sizeof(char *) * g->names_capacity * 2);
    g->names_capacity *= 2;
    assert(g->names != 0);
  }

  g->names[g->names_count] = (char *) calloc(name_len + 1, sizeof(char));
  memcpy(g->names[g->names_count], name, name_len);
  ++(g->names_count);

  return(g->names_count - 1);
}

static void
add_nonterminal(grammar *g, char *name, uint32_t name_len)
{
  if (contains(&(g->symbols), name, name_len))
  {
    return;
  }

  push_back(&(g->nonterm_names_idx), add_name(g, name, name_len));
  set(&(g->symbols), name, name_len, ++(g->nonterm_count));

  if (g->nonterm_count > g->rule_capacity)
  {
    uint32_t old_capacity = g->rule_capacity;
    g->rules = (rule **) realloc(g->rules, sizeof(rule *) * g->rule_capacity * VECTOR_SIZE_MULTIPLIER);
    g->rule_capacity *= VECTOR_SIZE_MULTIPLIER;
    assert(g->rules != 0);

    // NOTE(aolo2): init new rules with null ptr
    for (uint32_t i = old_capacity; i < g->rule_capacity; ++i)
    {
      g->rules[i] = 0;
    }
  }
}

static inline void
add_terminal(grammar *g, char *name, uint32_t name_len)
{
  if (!contains(&(g->symbols), name, name_len))
  {
    push_back(&(g->term_names_idx), add_name(g, name, name_len));
    set(&(g->symbols), name, name_len, -1 * ++(g->term_count));
  }
}

static inline int32_t
get_id(grammar *g, char *name, uint32_t name_len)
{
  return(get(&(g->symbols), name, name_len));
}

static void
_add_nonterm_rule(grammar *g, rule *r)
{
  uint32_t id = r->left;

  if (g->rules[id] == 0)
  {
    g->rules[id] = r;
  }
  else
  {
    rule *pr = g->rules[id];
    while (pr->next != 0)
    {
      // TODO(aolo2): fix this
      /*if (pr == r)
      {
        printf("[WARNING] Do not add the same rule twice!\n");
        return;
      }*/
      pr = pr->next;
    }
    pr->next = r;
  }
}

static void
add_rule(grammar *g, rule *r)
{
  assert(r->left <= g->nonterm_count);
  if (r->right.size == 0)
  {
    printf("[WARNING] Do not add rules with empty rhs!\n");
  }
  else
  {
    _add_nonterm_rule(g, r);
  }
}

static inline rule*
get_rules(grammar *g, uint32_t id)
{
  assert(id <= g->nonterm_count);
  return(g->rules[id]);
}

static vector
_first_rec(grammar *g, int32_t id, rule *r)
{
  int32_t sym = r->right.data[0];
  if (is_terminal(sym))
  {
    vector res = init_vector(1);
    push_back(&res, sym);
    return(res); // NOTE(aolo2): terminal
  }
  else
  {
    return(_first(g, sym));
  }
}

static vector
_first(grammar *g, int32_t id)
{
  vector res = init_vector(3);

  if (!is_terminal(id))
  {
    rule *pr = g->rules[id];
    while (pr != 0)
    {
      vector other = _first_rec(g, id, pr);
      append(&res, &other);
      free_vector(&other);
      pr = pr->next;
    }
  }
  else
  {
    push_back(&res, id);
  }

  return(res);
}

static inline void
_add_transition(grammar *g, uint32_t nonterm_id, int32_t term_id, vector *symbols)
{
  vector *pold = g->transition[nonterm_id] + term_id;
  if (pold->data[0] == get_id(g, "ERROR", 5))
  {
    pop_back(pold);
    append(pold, symbols);
  }
  else
  {
    append(pold, symbols);
  }
}

static void
compute_transition(grammar *g)
{
  g->transition = (vector **) malloc(sizeof(vector *) * (g->nonterm_count + 1));
  assert(g->transition != 0);

  // NOTE(aolo2): init with ERROR
  for (uint32_t i = 0; i < g->nonterm_count + 1; ++i)
  {
    g->transition[i] = (vector *) malloc(sizeof(vector) * (g->term_count + 1));
    assert(g->transition[i] != 0);
    for (uint32_t j = 0; j < g->term_count + 1; ++j)
    {
      g->transition[i][j] = init_vector(5);
      push_back(&(g->transition[i][j]), get_id(g, "ERROR", 5));
    }
  }

  int32_t epsilon = get_id(g, "epsilon", 7);

  unique_set all_first[g->nonterm_count + 1];
  unique_set all_follow[g->nonterm_count + 1];

  // NOTE(aolo2): init all follow sets as empty
  for (uint32_t i = 0; i < g->nonterm_count + 1; ++i)
  {
    all_first[i] = init_set();
    all_follow[i] = init_set();
  }

  // NOTE(aolo2): for all rules
  for (uint32_t i = 0; i < g->nonterm_count + 1; ++i)
  {
    vector x_first = _first(g, i);
    add(all_first + i, &x_first, false, 0);
    free_vector(&x_first);

    rule *pr = g->rules[i];
    while (pr != 0)
    {
      // NOTE(aolo2): seek 'X ::= uYv' rules
      // NOTE(aolo2): pt->right.size is always > 0
      for (uint32_t j = 0; j < pr->right.size - 1; ++j)
      {
        int32_t Y = pr->right.data[j];
        if (!is_terminal(Y))
        {
          vector v_first = _first(g, pr->right.data[j + 1]);
          add(all_follow + Y, &v_first, true, -1);
          free_vector(&v_first);
        }
      }

      pr = pr->next;
    }
  }

  // NOTE(aolo2): propogate all follow's
  bool changes_made = true;
  while (changes_made)
  {
    changes_made = false;
    for (uint32_t i = 0; i < g->nonterm_count + 1; ++i)
    {
      rule *pr = g->rules[i];
      while (pr != 0)
      {
        for (uint32_t j = 0; j < pr->right.size; ++j)
        {
          int32_t Y = pr->right.data[j];
          if (!is_terminal(Y))
          {
            bool is_last = (j == pr->right.size - 1);
            bool next_is_terminal;
            int32_t next_symbol;

            if (!is_last)
            {
              next_symbol = pr->right.data[j + 1];
              next_is_terminal = is_terminal(next_symbol);
            }

            if (is_last || (next_is_terminal && next_symbol == epsilon) ||
             (!next_is_terminal && has(all_first + next_symbol, epsilon)))
            {
              vector x_follow = items(all_follow + i);
              bool changed_now = add(all_follow + Y, &x_follow, false, 0);
              changes_made = changes_made || changed_now;
              free_vector(&x_follow);
            }
          }
        }
        pr = pr->next;
      }
    }
  }

#if 0
  printf("\n=== FOLLOW ===\n");
  for (uint32_t i = 0; i < g->nonterm_count + 1; ++i)
  {
    vector follow_items = items(all_follow + i);
    print_vector(&follow_items);
    printf("\n");
    free_vector(&follow_items);
  }
#endif

  // NOTE(aolo2): compute the transition table
  for (uint32_t i = 0; i < g->nonterm_count + 1; ++i)
  {
    rule *pr = g->rules[i];
    while (pr != 0)
    {
      vector u = pr->right;
      vector u_first = _first(g, u.data[0]);
      bool has_epsilon = false;

      for (uint32_t j = 0; j < u_first.size; ++j)
      {
        int32_t a = u_first.data[j];
        _add_transition(g, i, abs(a), &u);
        has_epsilon = has_epsilon || (a == epsilon);
      }

      free_vector(&u_first);

      if (has_epsilon)
      {
        vector x_follow = items(all_follow + i);
        for (uint32_t j = 0; j < x_follow.size; ++j)
        {
          int32_t b = x_follow.data[j];
          _add_transition(g, i, abs(b), &u);
        }
        free_vector(&x_follow);
      }

      pr = pr->next;
    }
  }

  for (uint32_t i = 0; i < g->nonterm_count + 1; ++i)
  {
    free_set(all_first + i);
    free_set(all_follow + i);
  }
}

static inline void
print_rule(rule *r)
{
  printf("%d ::= ", r->left);
  print_vector(&(r->right));
}

static void
print_grammar(grammar *g)
{
  printf("\n=== GRAMMAR ===\n");
  printf("Non-terminal count: %d\n", g->nonterm_count);
  printf("Terminal count: %d\n", g->term_count);
  printf("Axiom: %s\n", g->names[g->nonterm_names_idx.data[g->axiom - 1]]);

  printf("Rules:\n");
  for (uint32_t i = 0; i < g->nonterm_count + 1; ++i)
  {
    rule *r = g->rules[i];
    while (r != 0)
    {
      print_rule(r);
      printf("\n");
      r = r->next;
    }
  }

  printf("Table:\n");
  for (uint32_t i = 2; i < g->nonterm_count + 1; ++i)
  {
    printf("%-15s ", g->names[g->nonterm_names_idx.data[i - 1]]);
    for (uint32_t j = 2; j < g->term_count + 1; ++j)
    {
      char buffer[256];
      char *head = buffer;
      head += sprintf(head, "(%s)[", g->names[g->term_names_idx.data[j - 1]]);
      for (uint32_t k = 0; k < g->transition[i][j].size; ++k)
      {
        uint32_t symbol = g->transition[i][j].data[k];
        vector idx_vector = (is_terminal(symbol) ? g->term_names_idx : g->nonterm_names_idx);
        head += sprintf(head, " %s", g->names[idx_vector.data[abs(symbol) - 1]]);
      }
      head += sprintf(head, " ]  ");
      printf("%-55s", buffer);
    }
    printf("\n");
  }

  printf("Terminals:\n");
  for (uint32_t i = 0; i < g->term_count; ++i)
  {
    printf("%d: %s\n", (-1 * i) - 1, g->names[g->term_names_idx.data[i]]);
  }

  printf("Non-terminals:\n");
  for (uint32_t i = 0; i < g->nonterm_count; ++i)
  {
    printf("%d: %s\n", i + 1, g->names[g->nonterm_names_idx.data[i]]);
  }
}

static void
free_rules(grammar *g, uint32_t id)
{
  rule *pr = g->rules[id];

  while (pr != 0)
  {
    rule *next = pr->next;
    free_vector(&(pr->right));
    free(pr);
    pr = next;
  }
}

static void
free_grammar(grammar *g)
{
  for (uint32_t i = 0; i < g->rule_capacity; ++i)
  {
    if (g->rules[i] != 0)
    {
      free_rules(g, i);
    }
  }

  free(g->rules);
  free_hashmap(&(g->symbols));

  for (uint32_t i = 0; i < g->nonterm_count + 1; ++i)
  {
    for (uint32_t j = 0; j < g->term_count + 1; ++j)
    {
      free_vector(&(g->transition[i][j]));
    }
    free(g->transition[i]);
  }

  for (uint32_t i = 0; i < g->names_count; ++i)
  {
    free(g->names[i]);
  }

  free(g->names);
  free(g->transition);

  free_vector(&(g->term_names_idx));
  free_vector(&(g->nonterm_names_idx));

  g->term_count = 0;
  g->nonterm_count = 0;
  g->axiom = -1;
  g->rule_capacity = 0;
}

#endif