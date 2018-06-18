%option noyywrap bison-bridge bison-locations

%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int8_t TAG_NUMBER = 1;
const int8_t TAG_IDENT = 2;

const char * const TAG_NAMES[] = 
{
  "",
  "NUMBER",
  "IDENT"
};

typedef struct Position
{
  uint32_t line;
  uint32_t pos;
  uint32_t index;
} Position;

typedef struct Fragment
{
  Position starting;
  Position following;
} Fragment;

typedef Fragment YYLTYPE;

typedef union Token
{
  const char *text;
  uint64_t number;
} Token;

typedef union Token YYSTYPE;

int continued;
Position cur;

#define YY_USER_ACTION             \
{                                  \
  int i;                           \
  if (!continued)                  \
  {                                \
    yylloc->starting = cur;        \
  }                                \
  continued = 0;                   \
  for (i = 0; i < yyleng; ++i)     \
  {                                \
    if (yytext [i] == '\n')        \
    {                              \
      ++cur.line;                  \
      cur.pos = 1;                 \
    }                              \
    else                           \
    {                              \
      ++cur.pos;                   \
    }                              \
    ++cur.index;                   \
  }                                \
  yylloc->following = cur;         \
}

void init_scanner(char *program)
{
  continued = 0;
  cur.line = 1;
  cur.pos = 1;
  cur.index = 0;
  yy_scan_string(program);
}

uint64_t fibonacci(uint32_t n)
{
  return((n == 0) ? 1 : ((n == 1) ? 2 : fibonacci(n - 1) + fibonacci(n - 2)));
}

%}

CONSONANT [a-zA-Z]{-}[aeiouyAEIOUY]
VOWEL [aeiouyAEIOUY]

FIBONACCI_NUMBER 1?(01|0)+|1
ALTERNATING_IDENT ({CONSONANT}({VOWEL}{CONSONANT})*{VOWEL}?)|({VOWEL}({CONSONANT}{VOWEL})*{CONSONANT}?)

%%
[\n\t ]+

{ALTERNATING_IDENT}         {
                              yylval->text = yytext;

                              return(TAG_IDENT);
                            }

{FIBONACCI_NUMBER}          {
                              uint64_t current;
                              uint64_t sum = 0;

                              for (int i = 0; i < yyleng; ++i)
                              {
                                current = 0;
                                if (yytext[yyleng - 1 - i] == '1') current = fibonacci(i);
                                if (current > UINT64_MAX - sum)
                                {
                                  printf("ERROR: OVERFLOW");
                                  BEGIN(0);
                                }
                                sum += current;
                              }

                              yylval->number = sum;
                              return(TAG_NUMBER);
                            }

%%

int
main(int argc, char **argv)
{
  FILE *file;
  uint64_t size = 0;
  char *buffer;

  if (argc < 2)
  {
    printf("> Usage: ./lab6 INPUT\n");
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

  int tag;
  YYSTYPE value;
  YYLTYPE coords;
  
  init_scanner(buffer);
  
  do
  {
    tag = yylex(&value, &coords);
    if (tag != 0)
    {
      printf("%s (%d, %d)-(%d, %d)", TAG_NAMES[tag],
                                     coords.starting.line,
                                     coords.starting.pos,
                                     coords.following.line,
                                     coords.following.pos);
      if (tag == TAG_IDENT)
      {
        printf(" %s\n", value.text);
      }
      else if (tag == TAG_NUMBER)
      {
        printf(" %lu\n", value.number);
      }
    }
  } while (tag != 0);
  
  free(buffer); 

  return(0);
}