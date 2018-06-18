#ifndef LEXEM_H
#define LEXEM_H

#include "vector.h"

typedef enum
{
  ERROR,
  GRAMMAR,
  N_LIST,
  N_LIST_ITEMS,
  N_LIST_ITEMS_2,
  NON_TERMINAL,
  T_LIST,
  T_LIST_ITEMS,
  T_LIST_ITEMS_2,
  TERMINAL,
  RULES,
  RULES_2,
  RULE,
  RIGHT_PART,
  ALTERNATIVES,
  ALTERNATIVE,
  TN,
  TN_LIST,
  TN_LIST_2,
  AXIOM
} N;

typedef enum
{
  EOP,
  K_NONTERMINAL,
  K_TERMINAL,
  K_AXIOM,
  K_EPSILON,
  OR,
  QUAD_EQUALS,
  QUOTED_SYMBOL,
  SEMICOLON,
  COMMA,
  NT_NAME,
  LC_WORD,
  LEX_ERROR
} T;

static inline bool
is_terminal(int32_t s)
{
  return(s < 0);
}

static char*
t_string(T tag)
{
  switch(tag)
  {
    case EOP: return("EOP");
    case K_NONTERMINAL: return("K_NONTERMINAL");
    case K_TERMINAL: return("K_TERMINAL");
    case K_AXIOM: return("K_AXIOM");
    case K_EPSILON: return("K_EPSILON");
    case OR: return("OR");
    case QUAD_EQUALS: return("QUAD_EQUALS");
    case QUOTED_SYMBOL: return("QUOTED SYMBOL");
    case SEMICOLON: return("SEMICOLON");
    case COMMA: return("COMMA");
    case NT_NAME: return("NT_NAME");
    case LC_WORD: return("LC_WORD");
    case LEX_ERROR: return("LEXICAL ERROR");
    default: return("UNKOWN"); // NOTE(aolo2): should not happen
  }
}

static char*
n_string(N tag)
{
  switch (tag)
  {
    case GRAMMAR: return("GRAMMAR");
    case N_LIST: return("N_LIST");
    case N_LIST_ITEMS: return("N_LIST_ITEMS");
    case N_LIST_ITEMS_2: return("N_LIST_ITEMS_2");
    case NON_TERMINAL: return("NON_TERMINAL");
    case T_LIST: return("T_LIST");
    case T_LIST_ITEMS: return("T_LIST_ITEMS");
    case T_LIST_ITEMS_2: return("T_LIST_ITEMS_2");
    case TERMINAL: return("TERMINAL");
    case RULES: return("RULES");
    case RULES_2: return("RULES_2");
    case RULE: return("RULE");
    case RIGHT_PART: return("RIGHT_PART");
    case ALTERNATIVES: return("ALTERNATIVES");
    case ALTERNATIVE: return("ALTERNATIVE");
    case TN: return("TN");
    case TN_LIST: return("TN_LIST");
    case TN_LIST_2: return("TN_LIST_2");
    case AXIOM: return("AXIOM");
    default: return("UNKOWN"); // NOTE(aolo2): should not happen
  }
}

static char*
symbol_string(int32_t sym)
{
  return(is_terminal(sym) ? t_string((T) (-1 * sym)) : n_string((N) sym));
}

typedef struct
{
  uint32_t row;
  uint32_t column;
  uint32_t index;
  uint32_t len;
  char *text;
} position;

typedef struct
{
  position starting;
  position following;
} fragment;

typedef struct
{
  T tag;
  bool context;
  fragment coords;
} token;

typedef struct
{
  char *program;
  uint32_t len;
  position cur;
  bool ignore_keywords;
} scanner;

static position
init_position(char *text, uint32_t len)
{
  position p;

  p.text = text;
  p.len = len;
  p.row = 1;
  p.column = 1;
  p.index = 0;

  return(p);
}

static fragment
init_fragment(position starting, position following)
{
  fragment f;

  f.starting = starting;
  f.following = following;

  return(f);
}

static token
init_token(T tag, bool context, fragment coords)
{
  token t;

  t.tag = tag;
  t.context = context;
  t.coords = coords;

  return(t);
}

static scanner
init_scanner(char *program, uint32_t len)
{
  scanner sc;

  sc.program = program;
  sc.len = len;
  sc.cur = init_position(program, len);
  sc.ignore_keywords = false;

  return(sc);
}

static void
print_position(position *pos)
{
  printf("(%d, %d)", pos->row, pos->column);
}

static char
cp(position *pos)
{
  return((pos->index >= pos->len) ? -1 : pos->text[pos->index]);
}

static bool
is_whitespace(position *pos)
{
  char c = cp(pos);
  return(c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

inline static bool
is_capital_letter(position *pos)
{
  char c = cp(pos);
  return(c >= 'A' && c <= 'Z');
}

inline static bool
is_letter(position *pos)
{
  char c = cp(pos);
  return((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

inline static bool
is_lc_letter(position *pos)
{
  char c = cp(pos);
  return(c >= 'a' && c <= 'z');
}

inline static bool
is_digit(position *pos)
{
  char c = cp(pos);
  return(c >= '0' && c <= '9');
}

inline static bool
is_allowed_spec(position *pos)
{
  char c = cp(pos);
  return(c == '_' || c == '-' || c == ':' || c == '=');
}

static bool
is_newline(position *pos)
{
  char c = cp(pos);
  if (c == '\r' && pos->index < pos->len - 1)
  {
    return(pos->text[pos->index + 1] == '\n');
  }
  return(c == '\n');
}

static void
inc(position *pos)
{
  if (pos->index < pos->len)
  {
    if (is_newline(pos))
    {
      if (pos->text[pos->index] == '\r')
      {
        ++(pos->index);
      }
      ++(pos->row);
      pos->column = 1;
    }
    else
    {
      ++(pos->column);
    }
    ++(pos->index);
  }
}

static bool
keyword(scanner *sc, char *word)
{
  if (sc->ignore_keywords)
  {
    return(false);
  }

  uint32_t wlen = strlen(word);
  uint32_t cur_idx = sc->cur.index;

  assert(wlen > 0);

  if (sc->len - cur_idx >= wlen)
  {
    return(strncmp(&(sc->program[cur_idx]), word, wlen) == 0);
  }
  else
  {
    return(false);
  }
}

static inline void
seek(scanner *sc, uint32_t n)
{
  for (uint32_t i = 0; i < n; ++i)
  {
    inc(&(sc->cur));
  }
}

static token
next_token(scanner *sc)
{
  while (cp(&(sc->cur)) != -1)
  {
    while (is_whitespace(&(sc->cur)))
    {
      inc(&(sc->cur));
    }

    if (cp(&(sc->cur)) == -1)
    {
      break;
    }

    position start = sc->cur;

    switch (cp(&(sc->cur)))
    {
      case ',':
      {
        /* COMMA */
        inc(&(sc->cur));
        return(init_token(COMMA, false, init_fragment(start, sc->cur)));
      }
      break;
      case ';':
      {
        /* SEMICOLON */
        inc(&(sc->cur));
        sc->ignore_keywords = false;
        return(init_token(SEMICOLON, false, init_fragment(start, sc->cur)));
      }
      break;
      case '|':
      {
        inc(&(sc->cur));
        return(init_token(OR, false, init_fragment(start, sc->cur)));
      }
      break;
      case '\'':
      {
        /* T_CONTENT */
        seek(sc, 2);
        if (cp(&(sc->cur)) == '\'')
        {
          inc(&(sc->cur));
          return(init_token(QUOTED_SYMBOL, true, init_fragment(start, sc->cur)));
        }
        else
        {
          return(init_token(LEX_ERROR, false, init_fragment(start, sc->cur)));
        }
      }
      break;
      case ':':
      {
        /* COLON OR QUAD_EQUALS */
        if (keyword(sc, "::="))
        {
          seek(sc, 3);
          sc->ignore_keywords = true;
          return(init_token(QUAD_EQUALS, false, init_fragment(start, sc->cur)));
        }
        // NOTE(aolo2): fall-through for reading '::=' as a string
        else if (!sc->ignore_keywords)
        {
          return(init_token(LEX_ERROR, false, init_fragment(start, sc->cur)));
        }
      }
      default:
      {
        if (keyword(sc, "non-terminal"))
        {
          seek(sc, 12);
          sc->ignore_keywords = true;
          return(init_token(K_NONTERMINAL, false, init_fragment(start, sc->cur)));
        }
        else if (keyword(sc, "terminal"))
        {
          seek(sc, 8);
          sc->ignore_keywords = true;
          return(init_token(K_TERMINAL, false, init_fragment(start, sc->cur)));
        }
        else if (keyword(sc, "epsilon"))
        {
          seek(sc, 7);
          return(init_token(K_EPSILON, false, init_fragment(start, sc->cur)));
        }
        else if (keyword(sc, "axiom"))
        {
          seek(sc, 5);
          return(init_token(K_AXIOM, false, init_fragment(start, sc->cur)));
        }
        else if (is_capital_letter(&(sc->cur)))
        {
          /* NT_NAME */
          while (is_digit(&(sc->cur)) ||
            is_capital_letter(&(sc->cur)) ||
            is_allowed_spec(&(sc->cur)))
          {
            inc(&(sc->cur));
          }
          return(init_token(NT_NAME, true, init_fragment(start, sc->cur)));
        }
        else if (is_letter(&(sc->cur)) || is_allowed_spec(&(sc->cur)))
        {
          /* LC_WORD */
          while (is_digit(&(sc->cur)) ||
            is_lc_letter(&(sc->cur)) ||
            is_allowed_spec(&(sc->cur)))
          {
            inc(&(sc->cur));
          }
          return(init_token(LC_WORD, true, init_fragment(start, sc->cur)));
        }
        else
        {
          return(init_token(LEX_ERROR, false, init_fragment(start, sc->cur)));
        }
      }
    }
  }

  return(init_token(EOP, false, init_fragment(sc->cur, sc->cur)));
}

static char*
read_file(char *filename)
{
  FILE *file;
  uint64_t size;
  char *buffer;

  file = fopen(filename, "rb");
  if (!file)
  {
    printf("# File could not be opened\n# Exiting\n");
    return(0);
  }

  fseek(file, 0L, SEEK_END);
  size = ftell(file);
  rewind(file);

  buffer = (char *) malloc(sizeof(char) * (size + 1));
  if (!buffer)
  {
    fclose(file);
    printf("# Malloc failed\n# Exiting\n");
    return(0);
  }

  if (fread(buffer, size, 1, file) != 1)
  {
    fclose(file);
    free(buffer);
    printf("# File read failed\n# Exiting\n");
    return(0);
  }

  buffer[size] = 0x00;
  printf("* Successfully read %lu bytes\n", size);
  fclose(file);

  return(buffer);
}

#endif