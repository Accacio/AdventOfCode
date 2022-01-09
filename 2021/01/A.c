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

  int inputNum, lastNum, counter;
  counter = -1;
  FILE *file = fopen (argv[1], "r");
  while (fscanf (file, "%d", &inputNum) != EOF)
    {
      if (inputNum > lastNum)
        counter++;
      lastNum = inputNum;
    }
  fclose (file);
  printf ("%d\n", counter);
  return 0;
}
