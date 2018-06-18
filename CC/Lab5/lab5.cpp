#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "token.cpp"

/*
Lab #5
  Lexical resolver

Var #31
  keywords - 'def', 'return'
  special symbols - '(', ')', ':'
  comments - """..."""
*/

/*
  ERROR 1 - bad comment
  ERROR 3 - unterminated comment
*/

const int8 TRANSITION[23][12] =
/*                     0        1           2      3           4      5    6    7    8    9    10   11 */
/*                     SPECIAL  WHITESPACE  DIGIT  LETTER_RD   QUOTE  R    E    T    U    N    D    F  */
{
/* 0. START       */{  1,      11,         12,    13,         2,     17,  13,  13,  13,  13,  14,  13  },
/* 1. SPECIAL     */{ -1,      -1,         -1,    -1,         -1,    -1,  -1,  -1,  -1,  -1,  -1,  -1  },
/* 2. COMMENT 1   */{  8,       8,          8,     8,          3,     8,   8,   8,   8,   8,   8,   8  },
/* 3. COMMENT 2   */{  8,       8,          8,     8,          4,     8,   8,   8,   8,   8,   8,   8  },
/* 4. COMMENT 3   */{  4,       4,          4,     4,          5,     4,   4,   4,   4,   4,   4,   4  },
/* 5. COMMENT 4   */{  4,       4,          4,     4,         6,      4,   4,   4,   4,   4,   4,   4 },
/* 6. COMMENT 5   */{  4,       4,          4,     4,         7,      4,   4,   4,   4,   4,   4,   4 },
/* 7. COMMENT 6   */{ -1,      -1,         -1,    -1,         -1,    -1,  -1,  -1,  -1,  -1,  -1,  -1  },
/* 8. ERROR 1     */{ -1,      -1,         -1,    -1,         -1,    -1,  -1,  -1,  -1,  -1,  -1,  -1  },
/* 9. ERROR 2     */{ -1,      -1,         -1,    -1,         -1,    -1,  -1,  -1,  -1,  -1,  -1,  -1  },
/* 10. ERROR 3    */{ -1,      -1,         -1,    -1,         -1,    -1,  -1,  -1,  -1,  -1,  -1,  -1  },
/* 11. WHITESPACE */{ -1,       11,        -1,    -1,         -1,    -1,  -1,  -1,  -1,  -1,  -1,  -1  },
/* 12. NUMBER     */{ -1,      -1,          12,   -1,         -1,     17,  13,  13,  13,  13,  14,  13 },
/* 13. IDENT      */{ -1,      -1,          13,    13,        -1,     13,  13,  13,  13,  13,  13,  13 },
/* 14. DEF 1      */{ -1,      -1,          13,    13,        -1,     13,  15,  13,  13,  13,  13,  13 },
/* 15. DEF 2      */{ -1,      -1,          13,    13,        -1,     13,  13,  13,  13,  13,  13,  16 },
/* 16. DEF 3      */{ -1,      -1,          13,    13,        -1,     13,  13,  13,  13,  13,  13,  13 },
/* 17. RETURN 1   */{ -1,      -1,          13,    13,        -1,     13,  18,  13,  13,  13,  13,  13 },
/* 18. RETURN 2   */{ -1,      -1,          13,    13,        -1,     13,  13,  19,  13,  13,  13,  13 },
/* 19. RETURN 3   */{ -1,      -1,          13,    13,        -1,     13,  13,  13,  20,  13,  13,  13 },
/* 20. RETURN 4   */{ -1,      -1,          13,    13,        -1,     21,  13,  13,  13,  13,  13,  13 },
/* 21. RETURN 5   */{ -1,      -1,          13,    13,        -1,     13,  13,  13,  13,  22,  13,  13 },
/* 22. RETURN 6   */{ -1,      -1,          13,    13,        -1,     13,  13,  13,  13,  13,  13,  13 },
};

internal STATE
transition(STATE state, DOMAIN gen_char)
{
  return (STATE) ((gen_char == DOMAIN::EOP) ?
                        -1 :
                        (TRANSITION[state][gen_char]));
}

internal DOMAIN
get_domain(char c)
{
  if (c == -1) return(DOMAIN::EOP);
  if (c == '(' || c == ')' || c == ':') return(DOMAIN::SPECIAL);
  if (c == ' ' || c == '\t' || c == '\n') return(DOMAIN::WHITESPACE);
  if (c >= '0' && c <= '9') return(DOMAIN::DIGIT);
  if (c == '\"') return(DOMAIN::QUOTE);
  if (c == 'r') return(DOMAIN::R);
  if (c == 'e') return(DOMAIN::E);
  if (c == 't') return(DOMAIN::T);
  if (c == 'u') return(DOMAIN::U);
  if (c == 'n') return(DOMAIN::N);
  if (c == 'd') return(DOMAIN::D);
  if (c == 'f') return(DOMAIN::F);
  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return(DOMAIN::LETTER_RD);
  return(DOMAIN::UNKNOWN);
}

internal char
cp(position &pos)
{
  return((pos.index >= pos.len) ? -1 : pos.text[pos.index]);
}

internal void
inc(position &pos)
{
  if (pos.index < pos.len)
  {
    if (cp(pos) == '\n')
    {
      ++(pos.line);
      pos.pos = 1;
    }
    else
    {
      ++(pos.pos);
    }
    ++(pos.index);
  }
}

internal const char*
print_token_name(token &tok)
{
  switch (tok.tag)
  {
    case (STATE::SPECIAL_S): return("SPECIAL");
    case (STATE::COMMENT_6): return("COMMENT");
    case (STATE::ERROR_1): return("BAD COMMENT");
    case (STATE::ERROR_3): return("UNTERMINATED COMMENT");
    case (STATE::WHITESPACE_S): return("WHITESPACE");
    case (STATE::NUMBER): return("NUMBER");
    case (STATE::IDENT): return("IDENT");
    case (STATE::DEF_1): return("IDENT");
    case (STATE::DEF_2): return("IDENT");
    case (STATE::DEF_3): return("DEF");
    case (STATE::RETURN_1): return ("IDENT");
    case (STATE::RETURN_2): return ("IDENT");
    case (STATE::RETURN_3): return ("IDENT");
    case (STATE::RETURN_4): return ("IDENT");
    case (STATE::RETURN_5): return ("IDENT");
    case (STATE::RETURN_6): return("RETURN");
    case (STATE::COMMENT_1): return("BAD COMMENT");
    case (STATE::COMMENT_2): return("BAD COMMENT");
    case (STATE::COMMENT_3): return("UNTERMINATED COMMENT");
    case (STATE::COMMENT_4): return("UNTERMINATED COMMENT");
    case (STATE::COMMENT_5): return("UNTERMINATED COMMENT");
    default: return("THIS SHOULD NOT HAPPED");
  }

  return("UNKNOWN");
}

internal bool
is_final(int8 state)
{
  return(state == -1);
}
  
internal token
next_token(scanner &sc)
{
  STATE state = transition(STATE::START, get_domain(cp(sc.cur)));
  STATE last_state = state;
  position start = sc.cur;
  DOMAIN gen_char;

  while (!is_final(state))
  {
    inc(sc.cur);
    last_state = state;
    gen_char = get_domain(cp(sc.cur));
    state = transition(state, gen_char);
  }

  return(init_token(last_state, init_fragment(start, sc.cur)));
}

int
main(int argc, char **argv)
{
  FILE *file;
  uint64 size = 0;
  char *buffer;

  if (argc < 2)
  {
    printf("> Usage: ./lab5 INPUT\n");
    return(0);
  }

  file = fopen(argv[1], "rb");
  if (!file)
  {
    printf("# File could not be opened\n# Exiting\n");
    return(1);
  }

  fseek(file, 0L, SEEK_END);
  size = ftell(file);
  rewind(file);

  buffer = (char *) malloc(sizeof(char) * (size + 1));
  if (!buffer)
  {
    fclose(file);
    printf("# Malloc failed\n# Exiting\n");
    return(1);
  }

  if (fread(buffer, size, 1, file) != 1)
  {
    fclose(file);
    free(buffer);
    printf("# File read failed\n# Exiting\n");
    return(1);
  }

  buffer[size] = 0x00;
  printf("* Successfully read %lu bytes\n", size);
  fclose(file);

  scanner sc = init_scanner(buffer, size);
  token tok = {};

  while (true)
  {
    tok = next_token(sc);
    if (tok.tag == -1)
    {
      break;
    }

    if (tok.tag != STATE::WHITESPACE_S)
    {
      uint32 attr_len = tok.coords.following.index - tok.coords.starting.index;
      printf("%s (%d,%d)-(%d,%d): %.*s", print_token_name(tok), tok.coords.starting.line,
                                                            tok.coords.starting.pos,
                                                            tok.coords.following.line,
                                                            tok.coords.following.pos, 
                                                            attr_len,
                                                            &tok.coords.starting.text[tok.coords.starting.index]);
      printf("\n");
    }
  }

  free(buffer);
  return(0);
}