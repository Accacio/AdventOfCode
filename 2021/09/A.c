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

  long count = 0;
  int cols = 100;
  int rows = 100;
  int map[rows + 2][cols + 2];
  for (int i = 0; i < rows + 2; i++)
    {
      for (int j = 0; j < cols + 2; j++)
        {
          map[i][j] = 9;
        }
    }

  FILE *file = fopen (argv[1], "r");
  char alg[2] = " \0";
  int iter = 0;
  while (fscanf (file, "%1s", alg) != EOF)
    {
      map[iter / (cols) + 1][iter % (cols) + 1] = atoi (alg);
      iter++;
    }

  for (int i = 1; i < rows + 1; i++)
    {
      for (int j = 1; j < cols + 1; j++)
        {
          if (map[i][j] < map[i][j - 1] && map[i][j] < map[i][j + 1]
              && map[i][j] < map[i - 1][j] && map[i][j] < map[i + 1][j])
            {
              printf ("\033[33m");
              count += 1 + map[i][j];
            }
          printf ("%d\033[0m", map[i][j]);
        }
      printf ("\n");
    }
  printf ("%ld\n", count);

  return 0;
}
