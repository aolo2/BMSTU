#pragma once

#include <vector>
#include <deque>
#include <string>
#include <cassert>

enum N
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
};

enum T
{
  EOP,
  K_NONTERMINAL,
  K_TERMINAL,
  K_AXIOM,
  K_EPSILON,
  OR,
  QUAD_EQUALS,
  QUOTE,
  SEMICOLON,
  COMMA,
  NT_NAME,
  LC_WORD,
  SYMBOL
};

struct position
{
  uint32_t row;
  uint32_t column;
  uint64_t index;
  std::string text;
};

struct fragment
{
  position starting;
  position following;
};

struct token
{
  int32_t tag;
  fragment coords;
};

struct scanner
{
  position cur;
  const std::string &program;
};


inline static std::string
token_payload(const token &t, const std::string &text)
{
  return(text.substr(t.coords.starting.index,
   t.coords.following.index - t.coords.starting.index));
}

static std::string
token_name(T tag)
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
    case QUOTE: return("QUOTE");
    case SEMICOLON: return("SEMICOLON");
    case COMMA: return("COMMA");
    case NT_NAME: return("NT_NAME");
    case LC_WORD: return("LC_WORD");
    case SYMBOL: return("SYMBOL");
    default: return("UNKOWN"); // NOTE(aolo2): should not happen
  }
}

static std::string
nonterm_name(N tag)
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
    case AXIOM: return("case");
    default: return("UNKOWN"); // NOTE(aolo2): should not happen
  }
}

static std::string
symbol_name(int32_t sym)
{
  return(sym < 0 ? token_name((T) (-1 * sym)) : nonterm_name((N) sym));
}

inline static position
init_position(const std::string &text)
{
  position p = { 1, 1, 0, text };
  return(p);
}

inline static fragment
init_fragment(position starting, position following)
{
  fragment f = { starting, following };
  return(f);
}

inline static token
init_token(int32_t tag, fragment coords)
{
  token t = { tag, coords };
  return(t);
}

inline static token
init_token(int32_t tag, position start, position follow)
{
  token t = { tag, init_fragment(start, follow) };
  return(t);
}

static scanner
init_scanner(const std::string &text)
{
  scanner sc = { init_position(text), text };
  return(sc);
}

inline static char
cp(position &pos)
{
  return((pos.index >= pos.text.size()) ? -1 : pos.text[pos.index]);
}

inline static bool
is_whitespace(position &pos)
{
  if (pos.index >= pos.text.size())
  {
    return(false);
  }

  char c = pos.text[pos.index];
  return(c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

inline static bool
is_capital_letter(position &pos)
{
  if (pos.index >= pos.text.size())
  {
    return(false);
  }

  char c = pos.text[pos.index];
  return(c >= 'A' && c <= 'Z');
}

inline static bool
is_letter(position &pos)
{
  if (pos.index >= pos.text.size())
  {
    return(false);
  }

  char c = pos.text[pos.index];
  return((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

inline static bool
is_lc_letter(position &pos)
{
  if (pos.index >= pos.text.size())
  {
    return(false);
  }

  char c = pos.text[pos.index];
  return(c >= 'a' && c <= 'z');
}

inline static bool
is_digit(position &pos)
{
  if (pos.index >= pos.text.size())
  {
    return(false);
  }

  char c = pos.text[pos.index];
  return(c >= '0' && c <= '9');
}

inline static bool
is_newline(position &pos)
{
  if (pos.index == pos.text.size())
  {
    return(true);
  }

  char c = pos.text[pos.index];
  if (c == '\r' && pos.index < pos.text.size() - 1)
  {
    return(pos.text[pos.index + 1] == '\n');
  }
  return(c == '\n');
}

static void
inc(position &pos)
{
  if (pos.index < pos.text.size())
  {
    if (is_newline(pos))
    {
      if (pos.text[pos.index] == '\r')
      {
        ++(pos.index);
      }
      ++(pos.row);
      pos.column = 1;
    }
    else
    {
      ++(pos.column);
    }
    ++(pos.index);
  }
}

static std::deque<token>
get_tokens(scanner &sc)
{
  std::deque<token> tokens;

  while (cp(sc.cur) != -1)
  {
    while (is_whitespace(sc.cur))
      inc(sc.cur);

    if (cp(sc.cur) == -1)
      break;

    position start = sc.cur;

    switch (cp(sc.cur))
    {
      case ':':
        /* COLON OR QUAD_EQUALS */
        if (sc.program.substr(sc.cur.index, 3) == "::=")
        {
          for (uint8_t i = 0; i < 3; ++i)
            inc(sc.cur);
          tokens.push_front(init_token(T::QUAD_EQUALS, start, sc.cur));
        }
        else
        {
            // TODO(aolo2): error
          inc(sc.cur);
        }
        break;
      case ',':
        /* COMMA */
        inc(sc.cur);
        tokens.push_front(init_token(T::COMMA, start, sc.cur));
        break;
      case ';':
        /* SEMICOLUMN */
        inc(sc.cur);
        tokens.push_front(init_token(T::SEMICOLON, start, sc.cur));
        break;
      case '|':
        inc(sc.cur);
        tokens.push_front(init_token(T::OR, start, sc.cur));
        break;
      case '\'':
        /* T_CONTENT */
        inc(sc.cur);
        tokens.push_front(init_token(T::QUOTE, start, sc.cur));
        start = sc.cur;

        inc(sc.cur);
        tokens.push_front(init_token(T::SYMBOL, start, sc.cur));
        start = sc.cur;

        if (cp(sc.cur) == '\'')
        {
          inc(sc.cur);
          tokens.push_front(init_token(T::QUOTE, start, sc.cur));
        }
        else
          assert(false);
            // TODO(aolo2): error
        break;
      default:
        if (sc.program.substr(sc.cur.index, 12) == "non-terminal")
        {
          for (uint8_t i = 0; i < 12; ++i)
            inc(sc.cur);
          tokens.push_front(init_token(T::K_NONTERMINAL, start, sc.cur));
        }
        else if (sc.program.substr(sc.cur.index, 8) == "terminal")
        {
          for (uint8_t i = 0; i < 8; ++i)
            inc(sc.cur);
          tokens.push_front(init_token(T::K_TERMINAL, start, sc.cur));
        }
        else if (sc.program.substr(sc.cur.index, 7) == "epsilon")
        {
          for (uint8_t i = 0; i < 7; ++i)
            inc(sc.cur);
          tokens.push_front(init_token(T::K_EPSILON, start, sc.cur));
        }
        else if (sc.program.substr(sc.cur.index, 5) == "axiom")
        {
          for (uint8_t i = 0; i < 5; ++i)
            inc(sc.cur);
          tokens.push_front(init_token(T::K_AXIOM, start, sc.cur));
        }
        else if (is_capital_letter(sc.cur))
        {
          /* NT_NAME */
          while (is_digit(sc.cur) || is_capital_letter(sc.cur))
            inc(sc.cur);

          tokens.push_front(init_token(T::NT_NAME, start, sc.cur));
        }
        else if (is_letter(sc.cur))
        {
            /* LC_WORD */
          while (is_digit(sc.cur) || is_lc_letter(sc.cur))
            inc(sc.cur);

          tokens.push_front(init_token(T::LC_WORD, start, sc.cur));
        }
    }
  }
  tokens.push_front(init_token(T::EOP, init_fragment(sc.cur, sc.cur)));
  return(tokens);
}

static std::deque<token>
lex(const std::string &text, std::vector<std::string> &errors)
{
  std::deque<token> tokens;
  scanner sc = init_scanner(text);
  return(get_tokens(sc));
}

static inline bool
is_terminal(const int32_t &s)
{
  return(s < 0);
}

/*static std::ostream&
operator<<(std::ostream &o, const position &p)
{
  o << "(" << p.row << ", " << p.column << ")";
  return(o);
}*/