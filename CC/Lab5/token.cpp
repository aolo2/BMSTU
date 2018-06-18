#include <cstdint>

#define internal static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

enum DOMAIN
{
  SPECIAL,
  WHITESPACE,
  DIGIT,
  LETTER_RD,
  QUOTE,
  R, E, T, U, N, D, F,
  EOP,
  UNKNOWN
};

enum STATE
{
  START, 
  SPECIAL_S,
  COMMENT_1,
  COMMENT_2,
  COMMENT_3,
  COMMENT_4,
  COMMENT_5,
  COMMENT_6,
  ERROR_1,
  ERROR_2,
  ERROR_3,
  WHITESPACE_S,
  NUMBER, 
  IDENT, 
  DEF_1,
  DEF_2,
  DEF_3,
  RETURN_1,
  RETURN_2,
  RETURN_3,
  RETURN_4,
  RETURN_5,
  RETURN_6
};

struct position
{
  uint32 line;
  uint32 pos;
  uint32 index;
  uint32 len;
  char *text;
};

struct fragment
{
  position starting;
  position following;
};

struct token
{
  STATE tag;
  fragment coords;
};

struct scanner
{
  char *program;
  uint32 len;
  position cur;
};

internal position
init_position(char *text, uint32 len)
{
  position p = {};
  p.text = text;
  p.len = len;
  p.line = 1;
  p.pos = 1;
  p.index = 0;
  return(p);
}

internal fragment
init_fragment(position starting, position following)
{
  fragment f = {};
  f.starting = starting;
  f.following = following;
  return(f);
}

internal token
init_token(STATE tag, fragment coords)
{
  token t = {};
  t.tag = tag;
  t.coords = coords;
  return(t);
}

internal scanner
init_scanner(char *program, uint32 len)
{
  scanner sc = {};
  sc.program = program;
  sc.len = len;
  sc.cur = init_position(program, len);
  return(sc);
}