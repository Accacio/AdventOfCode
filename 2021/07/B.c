#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long
pa_sum (long a)
{
  return (a + 1) * a / 2;
}

long
max_array (int *array, int count)
{
  long maximum = array[0];
  for (int i = 1; i < count; i++)
    {
      if (array[i] > maximum)
        maximum = array[i];
    }
  return maximum;
}

long
min_array (int *array, int count)
{
  long minimum = array[0];
  for (int i = 1; i < count; i++)
    {
      if (array[i] < minimum)
        minimum = array[i];
    }
  return minimum;
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
  fscanf (file, "%ms", &line);

  int *numbers;
  numbers = (int *) malloc (strlen (line) * sizeof (int));
  memset (numbers, 0, strlen (line));

  char *saveptr;
  char *token;
  int count = 0;
  for (char *str = line;; str = NULL, count++)
    {
      token = strtok_r (str, ",", &saveptr);
      if (!token)
        break;
      numbers[count] = atoi (token);
    }
  numbers = realloc (numbers, count * sizeof (int));

  long minimum;
  long minimumidx;
  for (long i = min_array (numbers, count); i <= max_array (numbers, count);
       i++)
    {
      long sum = 0;
      for (int j = 0; j < count; j++)
        {
          sum += pa_sum (abs (i - numbers[j]));
          if (sum >= minimum)
            {
              break;
            }
        }
      if (sum < minimum)
        {
          minimum = sum;
          minimumidx = i;
        }
    }
  printf ("Fuel: \033[34m%ld\033[0m | position: %ld\n", minimum, minimumidx);

  return 0;
}
