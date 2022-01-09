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
  long value = -1;
  int type = read_n_bits (*svptr, svptr, 3, count);
  printf ("%*sversion: %ld\n", 2 * depth, "", version);
  printf ("%*stype: %d\n", 2 * depth, "", type);
  if (type == 4)
    {
      value = read_literal (*svptr, svptr, count);
      printf ("%*sliteral: %ld\n", 2 * depth, "", value);
      /* return version; */
    }
  else
    {
      int length_id_type = read_n_bits (*svptr, svptr, 1, count);
      /* printf ("length_type: %d\n", length_id_type); */
      int n;
      if (length_id_type)
        n = read_n_bits (*svptr, svptr, 11, count);
      else
        n = read_n_bits (*svptr, svptr, 15, count);

      while (n > 0)
        {
          long m = 0;
          long sub = read_packet (*svptr, svptr, &m, depth + 1);
          if (value < 0)
            value = sub;
          else
            {
              switch (type)
                {
                case 0:
                  value += sub;
                  break;
                case 1:
                  value *= sub;
                  break;
                case 2:
                  value = (value < sub) ? value : sub;
                  break;
                case 3:
                  value = (value > sub) ? value : sub;
                  break;
                case 5:
                  value = value > sub;
                  break;
                case 6:
                  value = value < sub;
                  break;
                case 7:
                  value = (value == sub);
                  break;
                default:
                  break;
                }
            }

          *count += m;
          if (length_id_type)
            n -= 1;
          else
            n -= m;
        }
    }

  return value;
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

      /* printf ("bits: "); */
      /* print_bits (bits, bits_size); */
      /* printf ("length: %ld\n", strlen (line)); */
      char *svptr;
      long count;
      long version_sum = read_packet (bits, &svptr, &count, 0);
      printf ("version_sum: %ld\n", version_sum);

      free (line);
      free (bits);
      printf ("-------------- \n");
    }

  exit (EXIT_SUCCESS);
}
