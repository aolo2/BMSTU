#include <cstdio>
#include <cstdlib>

#include "token.cpp"

internal char
cp(position &pos)
{
  return((pos.index >= pos.len) ? -1 : pos.text[pos.index]);
}

internal bool
is_whitespace(position &pos)
{
  if (pos.index >= pos.len)
  {
    return(false);
  }

  char c = pos.text[pos.index];
  return(c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

internal bool
is_letter(position &pos)
{
  if (pos.index >= pos.len)
  {
    return(false);
  }

  char c = pos.text[pos.index];
  return((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

internal bool
is_digit(position &pos)
{
  if (pos.index >= pos.len)
  {
    return(false);
  }

  char c = pos.text[pos.index];
  return(c >= '0' && c <= '9'); 
}

internal bool
is_newline(position &pos)
{
  if (pos.index == pos.len)
  {
    return(true);
  }

  char c = pos.text[pos.index];
  if (c == '\r' && pos.index < pos.len - 1)
  {
    return(pos.text[pos.index + 1] == '\n');
  }
  return(c == '\n');
}

internal void
inc(position &pos)
{
  if (pos.index < pos.len)
  {
    if (is_newline(pos))
    {
      if (pos.text[pos.index] == '\r')
      {
        ++(pos.index);
      }
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
    case DOMAIN::WORD:
      return "WORD";
    case DOMAIN::LABEL:
      return "LABEL";
    case DOMAIN::COMMENT:
      return "COMMENT";
    case DOMAIN::EOP:
      return "EOP";
    case DOMAIN::OTHER:
      return "OTHER";
  }

  return "UNKNOWN";
}

internal token
next_token(scanner &sc)
{
  while (cp(sc.cur) != -1)
  {
    while (is_whitespace(sc.cur))
    {
      inc(sc.cur);
    }

    /* Trailing whitespace */
    if (cp(sc.cur) == -1)
    {
      break;
    }

    position start = sc.cur;
    char c = 0;

    switch (cp(sc.cur))
    {
      case ':':
        inc(sc.cur);

        if (cp(sc.cur) == ':')
        {
          /* Comment */
          inc(sc.cur);
          c = cp(sc.cur);

          while (c != -1 && c != '\r' && c != '\n')
          {
            inc(sc.cur);
            c = cp(sc.cur);
          }

          return(init_token(DOMAIN::COMMENT, init_fragment(start, sc.cur)));
        }

        while (is_digit(sc.cur) || is_letter(sc.cur))
        {
          inc(sc.cur);
        }

        return(init_token(DOMAIN::LABEL, init_fragment(start, sc.cur)));
      case '\"':
        inc(sc.cur);
        c = cp(sc.cur);

        while (c != '\"' && c != '\r' && c != -1)
        {
          c = cp(sc.cur);
          inc(sc.cur);
        }

        if (c == '\r')
        {
          inc(sc.cur);
        }

        return(init_token(DOMAIN::WORD, init_fragment(start, sc.cur)));
      default:
        c = cp(sc.cur);

        while (!is_whitespace(sc.cur) && c != -1)
        {
          c = cp(sc.cur);
          inc(sc.cur);
        }

        return(init_token(DOMAIN::WORD, init_fragment(start, sc.cur)));
    }
  }
  return(init_token(DOMAIN::EOP, init_fragment(sc.cur, sc.cur)));
}

int
main(int argc, char **argv)
{
  FILE *file;
  uint64 size;
  char *buffer;

  if (argc < 2)
  {
    printf("> Usage: ./lab4 INPUT\n");
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

  while (true)
  {
    token tok = next_token(sc);
    printf("%s (%d,%d)-(%d,%d)", print_token_name(tok), tok.coords.starting.line, 
                                                          tok.coords.starting.pos,
                                                          tok.coords.following.line, 
                                                          tok.coords.following.pos);
    
    if (tok.tag == DOMAIN::WORD || tok.tag == DOMAIN::LABEL || tok.tag == DOMAIN::COMMENT)
    {
      bool tail = (tok.coords.following.index >= sc.len);
      char mem = 0;
      if (!tail)
      {
        mem = sc.program[tok.coords.following.index];
        sc.program[tok.coords.following.index] = 0x00;
      }
      printf(": %s", &sc.program[tok.coords.starting.index]);
      if (!tail)
      {
        sc.program[tok.coords.following.index] = mem;
      }
    }
    printf("\n");

    if (tok.tag == DOMAIN::EOP)
    {
      break;
    }
  }

  free(buffer);

  return (0);
}