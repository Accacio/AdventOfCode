#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char
find_matching (char c)
{
  switch (c)
    {
    case '(': // Opening
      return ')';
      break;
    case '[':
      return ']';
      break;
    case '{':
      return '}';
      break;
    case '<':
      return '>';
      break;
    default:
      break;
    }
  return ' ';
}

long
find_points (char c)
{
  switch (c)
    {
    case ')':
      return 3;
      break;
    case ']':
      return 57;
      break;
    case '}':
      return 1197;
      break;
    case '>':
      return 25137;
      break;
    case ' ':
      break;
    }
  return 0;
}

int
main (int argc, char *argv[])
{
  if (!argv[1])
    {
      printf ("no file given\n");
      exit (EXIT_FAILURE);
    }

  FILE *file = fopen (argv[1], "r");
  char *line;
  char expecting[1000];
  int expectCount = 0;
  long points = 0;

  int found_error = 0;
  while (fscanf (file, "%ms", &line) != EOF)
    {
      /* printf("%s\n",line); */
      char c = 0;
      char *saveptr = line;
      do
        {
          sscanf (saveptr, "%c", &c);
          /* printf("%c",c); */

          switch (c)
            {
            case '(': // Opening
            case '[':
            case '{':
            case '<':
              /* printf("%c ",c); */
              expecting[expectCount] = c;
              expectCount++;
              break;
            default: // closing
              if (c == find_matching (expecting[expectCount - 1]))
                {
                  /* printf("%c ",find_matching(expecting[expectCount-1])); */
                  expectCount--;
                }
              else
                {
                  found_error = 1;
                  points += find_points (c);
                  /* printf(" Expected %c but found %c
                   * instead",find_matching(expecting[expectCount-1]),c); */
                }

              break;
            }
          saveptr++;
        }
      while (!found_error && saveptr != line + strlen (line));

      /* printf("\n\033[33mTotal: %d ",expectCount); */
      /* for (int i =0; i < expectCount; i++) { */
      /*     printf("%c ",expecting[i]); */
      /* } */
      /* printf("\033[0m\n"); */
      expectCount = 0;
      found_error = 0;
      /* printf("\n"); */
    }
  printf ("%ld\n", points);

  return 0;
}
