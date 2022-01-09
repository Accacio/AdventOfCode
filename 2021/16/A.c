#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
get_bits (char *line, char *bits)
{
  for (long i = 0; i < strlen (line); i++)
    {
      char c = *(line + i);
      int num = strtol (&c, NULL, 16);
      for (int j = 0; j < 4; j++)
        {
          bits[4 * i + j] = num >> (3 - j) & 1;
        }
    }
}

void
print_bits (char *bits, int bits_size)
{
  for (int i = 0; i < bits_size; i++)
    {
      printf ("%d", bits[i]);
    }
  printf ("\n");
}

long
read_n_bits (char *bits, char **svptr, long n, long *count)
{
  long ret = 0;
  for (long i = 0; i < n; i++)
    {
      ret = (ret << 1) + bits[i];
    }
  if (svptr != 0)
    *svptr = bits + n;

  (*count) += n;
  return ret;
}

long
read_literal_aux (char *bits, char **svptr, long *literal, long *count)
{

  long cont = bits[0];
  for (int i = 1; i < 5; i++)
    {
      *literal = (*literal << 1) + bits[i];
    }
  if (svptr != 0)
    *svptr = bits + 5;
  *count += 5;

  return cont;
}

long
read_literal (char *bits, char **svptr, long *count)
{
  long ret = 0;

  int cont = read_literal_aux (bits, svptr, &ret, count);
  while (cont)
    {
      cont = read_literal_aux (*svptr, svptr, &ret, count);
    }
  return ret;
}

long
read_packet (char *bits, char **svptr, long *count, int depth)
{
  long version = read_n_bits (bits, svptr, 3, count);
  int type = read_n_bits (*svptr, svptr, 3, count);
  printf ("%*sversion: %ld\n", 2 * depth, "", version);
  printf ("%*stype: %d\n", 2 * depth, "", type);
  if (type == 4)
    {
      long literal = read_literal (*svptr, svptr, count);
      printf ("%*sliteral: %ld\n", 2 * depth, "", literal);
    }
  else
    {
      int length_id_type = read_n_bits (*svptr, svptr, 1, count);
      if (length_id_type)
        {
          int n_subpackets = read_n_bits (*svptr, svptr, 11, count);
          printf ("%*snsub:  %d\n", 2 * depth, "", n_subpackets);
          while (n_subpackets > 0)
            {
              version += read_packet (*svptr, svptr, count, depth + 1);
              n_subpackets -= 1;
            }
        }
      else
        {
          int length_subpackets = read_n_bits (*svptr, svptr, 15, count);
          printf ("%*slength sub:  %d\n", 2 * depth, "", length_subpackets);
          while (length_subpackets > 0)
            {
              long m = 0;
              version += read_packet (*svptr, svptr, &m, depth + 1);
              length_subpackets -= m;
              *count += m;
            }
        }
    }
  return version;
}

int
main (int argc, char *argv[])
{
  if (!argv[1])
    {
      printf ("no file given\n");
      return (EXIT_FAILURE);
    }

  FILE *file = fopen (argv[1], "r");
  char *line;
  char *bits;

  while (fscanf (file, "%ms", &line) != EOF)
    {

      long bits_size = strlen (line) * 4;
      bits = malloc (bits_size);

      get_bits (line, bits);

      char *svptr;
      long count;
      long version_sum = read_packet (bits, &svptr, &count, 0);

      free (line);
      free (bits);
      printf ("\nversion_sum: %ld\n", version_sum);
      printf ("----------------- \n");
    }

  exit (EXIT_SUCCESS);
}
