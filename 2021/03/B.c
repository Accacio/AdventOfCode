#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

#define IS_BIT_SET(BF, N) ((BF >> N & 0x1))

typedef struct number
{
  long value;
  STAILQ_ENTRY (number) pointer;
} number;

typedef STAILQ_HEAD (number_list, number) number_list;

void
remove_from_list (number_list *list, int n, int select)
{
  int counttotal = 0;
  int count1 = 0;
  number *numberp;
  STAILQ_FOREACH (numberp, list, pointer)
  {
    count1 += IS_BIT_SET (numberp->value, n);
    counttotal++;
  }

  if (counttotal == 1)
    return;
  numberp = STAILQ_FIRST (list);
  STAILQ_FOREACH (numberp, list, pointer)
  {
    if (count1 >= (counttotal - count1)
        && !IS_BIT_SET (numberp->value, n) ^ select)
      {
        STAILQ_REMOVE (list, numberp, number, pointer);
      }
    else if (count1 < (counttotal - count1)
             && IS_BIT_SET (numberp->value, n) ^ select)
      {
        STAILQ_REMOVE (list, numberp, number, pointer);
      }
  }
  if (n == 0)
    return;
  return remove_from_list (list, n - 1, select);
}

int
main (int argc, char *argv[])
{
  if (!argv[1])
    {
      printf ("no file given\n");
      exit (EXIT_FAILURE);
    }
  number_list oxygen;
  STAILQ_INIT (&oxygen);

  number_list scrubber;
  STAILQ_INIT (&scrubber);

  int bit_columns = 12;

  char bit[bit_columns + 1];
  int totalNums = 0;

  FILE *file = fopen (argv[1], "r");
  while (fscanf (file, "%s", bit) != EOF)
    {
      number *number1 = malloc (sizeof (number));
      number1->value = strtol (bit, NULL, 2);
      STAILQ_INSERT_TAIL (&oxygen, number1, pointer);

      number *number2 = malloc (sizeof (number));
      number2->value = strtol (bit, NULL, 2);
      STAILQ_INSERT_TAIL (&scrubber, number2, pointer);

      totalNums++;
    }
  fclose (file);
  remove_from_list (&oxygen, bit_columns - 1, 1);
  remove_from_list (&scrubber, bit_columns - 1, 0);

  printf ("%ld\n", scrubber.stqh_first->value * oxygen.stqh_first->value);
  return 0;
}
