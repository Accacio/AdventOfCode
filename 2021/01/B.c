#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char *argv[])
{
  if (!argv[1])
    {
      printf ("no file given\n");
      exit (EXIT_FAILURE);
    }

  int inputNum, newSum, lastSum;
  int j = 1;
  int window[3] = { 0, 0, 0 };
  int counter = -1;

  FILE *file = fopen (argv[1], "r");
  fscanf (file, "%d", &inputNum);
  window[0] = inputNum;
  fscanf (file, "%d", &inputNum);
  window[1] = inputNum;

  lastSum = 0;
  for (int i = 0; i <= 2; i++)
    {
      lastSum += window[i];
    }

  while (fscanf (file, "%d", &inputNum) != EOF)
    {
      window[++j % 3] = inputNum;

      newSum = 0;
      for (int i = 0; i < 3; i++)
        {
          newSum += window[i];
        }
      if (newSum > lastSum)
        counter++;

      lastSum = newSum;
    }
  fclose (file);
  printf ("%d\n", counter);
  return 0;
}
