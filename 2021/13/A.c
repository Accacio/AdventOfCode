#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Paper
{
  int *map;
  int rows;
  int cols;
} Paper;

void
init_paper (Paper *paper, int rows, int cols)
{
  paper->rows = rows;
  paper->cols = cols;
  paper->map = (int *) malloc (rows * cols * sizeof (int));
  memset (paper->map, 0, rows * cols * sizeof (int));
}

void
print_paper (Paper paper)
{
  for (int i = 0; i < paper.rows; i++)
    {
      for (int j = 0; j < paper.cols; j++)
        {
          if (*(paper.map + j + i * paper.cols))
            printf ("#");
          else
            printf (".");
        }
      printf ("\n");
    }
}

long
count_paper (Paper paper)
{
  long ret = 0;
  for (int i = 0; i < paper.rows; i++)
    {
      for (int j = 0; j < paper.cols; j++)
        {
          int *cur = paper.map + j + i * paper.cols;
          ret += *cur ? 1 : 0;
        }
    }
  return ret;
}

void
fold_paper_vert (Paper *paper, int row)
{
  for (int i = row; i < paper->rows; i++)
    {
      for (int j = 0; j < paper->cols; j++)
        {
          int *cur = paper->map + j + i * paper->cols;
          if (*cur)
            {
              int corresponding_row = row - (i - row);
              *(paper->map + corresponding_row * paper->cols + j) = 1;
            }
        }
    }
  int oldrows = paper->rows;
  paper->rows = row;
  paper->map = realloc (paper->map, row * paper->cols * sizeof (int));
}

void
fold_paper_horz (Paper *paper, int col)
{
  long ret = 0;
  int *init = paper->map;
  int newcols = col;
  paper->map = malloc (newcols * paper->rows * sizeof (int));
  memset (paper->map, 0, newcols * paper->rows * sizeof (int));

  for (int i = 0; i < paper->rows; i++)
    {
      for (int j = 0; j < newcols; j++)
        {
          int *cur = init + j + i * paper->cols;
          int *new = paper->map + j + i *newcols;
          *new = *cur;
        }
    }
  for (int i = 0; i < paper->rows; i++)
    {
      for (int j = col; j < paper->cols; j++)
        {
          int *cur = init + j + i * paper->cols;
          int *new = paper->map + j + i * paper->cols;
          if (*cur)
            {
              int corresponding_col = col - (j - col);
              *(paper->map + corresponding_col + i * col) = 1;
            }
        }
    }
  paper->cols = col;
  free (init);
}

void
paper_add_value (Paper *paper, int col, int row)
{
  if (col >= paper->cols)
    {
      int *init = paper->map;
      int newcols = col + 1;
      paper->map = malloc ((newcols + 1) * paper->rows * sizeof (int));
      memset (paper->map, 0, (newcols + 1) * paper->rows * sizeof (int));

      for (int i = 0; i < paper->rows; i++)
        {
          for (int j = 0; j < paper->cols; j++)
            {
              int *cur = init + j + i * paper->cols;
              int *new = paper->map + j + i *(newcols);
              *new = *cur;
            }
        }
      paper->cols = newcols;
      free (init);
    }

  if (row >= paper->rows)
    {
      paper->map
          = realloc (paper->map, (row + 1) * paper->cols * sizeof (int));
      memset (paper->map + paper->rows * paper->cols, 0,
              (row + 1 - paper->rows) * paper->cols * sizeof (int));
      paper->rows = row + 1;
    }
  *(paper->map + col + row * paper->cols) = 1;
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

  Paper paper;
  init_paper (&paper, 1, 1);
  while (fscanf (file, "%ms", &line) != EOF)
    {
      if (line)
        {

          char *saveptr = line;
          char *xstr = strsep (&saveptr, ",");
          char *ystr = strsep (&saveptr, ",");
          if (!xstr || !ystr)
            break;
          paper_add_value (&paper, atoi (xstr), atoi (ystr));
          free (line);
        }
    }
  fscanf (file, "%ms", &line);
  free (line);
  fscanf (file, "%ms", &line);
  char *saveptr = line;
  char *dir = strsep (&saveptr, "=");
  char *size = strsep (&saveptr, "=");
  if (strcmp (dir, "x") == 0)
    fold_paper_horz (&paper, atoi (size));
  else
    fold_paper_vert (&paper, atoi (size));
  free (line);

  printf ("%ld\n", count_paper (paper));

  return 0;
}
