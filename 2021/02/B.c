#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main (int argc, char *argv[])
{
  if (!argv[1])
    {
      printf ("no file given\n");
      exit (EXIT_FAILURE);
    }
  int x, depth, aim;
  x = 0;
  depth = 0;
  aim = 0;
  int inputNum;
  char *string;
  string = malloc (50 * sizeof (char));
  FILE *file = fopen (argv[1], "r");
  while (fscanf (file, "%s %d", string, &inputNum) != EOF)
    {
      printf ("%s %d\n", string, inputNum);
      if (strcmp ("forward", string) == 0)
        {
          x += inputNum;
          depth += aim * inputNum;
        }
      else if (strcmp ("down", string) == 0)
        {
          aim += inputNum;
        }
      else if (strcmp ("up", string) == 0)
        {
          aim -= inputNum;
        }
      printf ("aim: %d depth: %d x: %d\n", aim, depth, x);
    }
  fclose (file);
  printf ("%ld\n", (long int) x * depth);
  return 0;
}
