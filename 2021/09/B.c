#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long
flood_fill (int *map, int *mark, int cols, int rows, int xInit, int yInit,
            int number)
{
  if (xInit < 0 || xInit > cols || yInit < 0 || yInit > rows
      || *(map + xInit + yInit * (cols)) == 9
      || *(mark + xInit + yInit * (cols)) != -1)
    return 0;
  *(mark + xInit + yInit * (cols)) = number;
  long ret = 1;
  ret += flood_fill (map, mark, cols, rows, xInit + 1, yInit, number);
  ret += flood_fill (map, mark, cols, rows, xInit - 1, yInit, number);
  ret += flood_fill (map, mark, cols, rows, xInit, yInit + 1, number);
  ret += flood_fill (map, mark, cols, rows, xInit, yInit - 1, number);

  return ret;
}

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
  int minX[cols * rows / 2];
  int minY[cols * rows / 2];
  int map[rows + 2][cols + 2];
  int mark[rows + 2][cols + 2];
  long sizeBasins[3] = { 1, 1, 1 };

  for (int i = 0; i < rows + 2; i++)
    {
      for (int j = 0; j < cols + 2; j++)
        {
          map[i][j] = 9;
          mark[i][j] = -1;
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
              minX[count] = j;
              minY[count] = i;
              count++;
            }
        }
    }

  for (int i = 0; i < count; i++)
    {
      long value = flood_fill (&map[0][0], &mark[0][0], cols + 2, rows + 2,
                               minX[i], minY[i], i);

      if (value > sizeBasins[2])
        {
          if (value >= sizeBasins[0])
            {
              sizeBasins[2] = sizeBasins[1];
              sizeBasins[1] = sizeBasins[0];
              sizeBasins[0] = value;
            }
          else if (value >= sizeBasins[1])
            {
              sizeBasins[2] = sizeBasins[1];
              sizeBasins[1] = value;
            }
          else
            {
              sizeBasins[2] = value;
            }
        }
    }
  for (int i = 1; i < rows + 1; i++)
    {
      for (int j = 1; j < cols + 1; j++)
        {
          if (mark[i][j] != -1)
            {
              int color = 32 + mark[i][j] % 5;
              printf ("\033[%dm", (color));
              printf ("%d\033[0m", 0);
            }
          else
            {
              printf ("9");
            }
        }
      printf ("\n");
    }

  printf ("%ld\n", sizeBasins[0] * sizeBasins[1] * sizeBasins[2]);

  return 0;
}
