#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
sort_array (long *array, long size)
{
  for (int i = 1; i < size; ++i)
    {
      for (int j = 0; j < (size - i); ++j)
        {
          if (array[j] > array[j + 1])
            {
              long temp = array[j];
              array[j] = array[j + 1];
              array[j + 1] = temp;
            }
        }
    }
}
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
      return 1;
      break;
    case ']':
      return 2;
      break;
    case '}':
      return 3;
      break;
    case '>':
      return 4;
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
  long allPoints[1000];
  int allPointsSize = 0;

  int found_error = 0;
  while (fscanf (file, "%ms", &line) != EOF)
    {
      /* printf("%s\n",line); */
      char c = 0;
      char *saveptr = line;
      do
        {
          sscanf (saveptr, "%c", &c);

          switch (c)
            {
            case '(': // Opening
            case '[':
            case '{':
            case '<':
              printf ("\033[32m%c\033[0m", c);
              expecting[expectCount] = c;
              expectCount++;
              break;
            default: // closing
              if (c == find_matching (expecting[expectCount - 1]))
                {
                  printf ("\033[32m%c\033[0m", c);
                  expectCount--;
                }
              else
                {
                  printf ("\033[31m%c\033[0m", c);
                  found_error = 1;
                }

              break;
            }
          saveptr++;
        }
      while (!found_error && saveptr != line + strlen (line));

      if (!found_error && expectCount > 0)
        {
          /* printf("\n\033[33mTotal: %d ",expectCount); */
          for (int i = expectCount - 1; i >= 0; i--)
            {
              points = 5 * points + find_points (find_matching (expecting[i]));
              printf ("\033[33m%c\033[0m", find_matching (expecting[i]));
            }
          /* printf("\033[0m\n"); */
          /* printf("Points \033[35m%ld\033[0m\n",points); */
          allPoints[allPointsSize] = points;
          allPointsSize++;
        }
      expectCount = 0;
      found_error = 0;
      points = 0;
      printf ("\n");
    }
  sort_array ((long *) &allPoints[0], allPointsSize);
  printf ("%ld\n", allPoints[allPointsSize / 2]);

  return 0;
}
