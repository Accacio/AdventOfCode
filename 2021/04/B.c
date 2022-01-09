#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BOARD
{
  int numbers[5][5];
  int marked[5][5];
  int won;
} Board;

void
initialize_board (Board *board)
{
  for (int i = 0; i < 5; i++)
    {
      memset (board->numbers[i], 0, 5 * sizeof (int));
      memset (board->marked[i], 0, 5 * sizeof (int));
      board->won = 0;
    }
}

void
print_board (Board board)
{
  for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 5; j++)
        {
          if (board.marked[i][j])
            {
              printf ("\033[33m");
            }
          printf ("%d\t", board.numbers[i][j]);
          printf ("\033[0m");
        }
      printf ("\n");
    }
}

int
read_board (Board *board, FILE *fp)
{
  int number;
  int totalNums = 0;
  while (totalNums < 5 * 5)
    {
      if (fscanf (fp, "%d ", &number) == EOF)
        return (EXIT_FAILURE);
      *(&board->numbers[0][0] + totalNums) = number;
      totalNums++;
    }
  return (EXIT_SUCCESS);
}

void
mark_board (Board *board, int number)
{
  for (int i = 0; i < 25; i++)
    {
      if (*(&board->numbers[0][0] + i) == number)
        {
          /* printf("is A_{%d%d}\n",i/5,i%5); */
          board->marked[i / 5][i % 5] = 1;
        }
    }
}

int
verify_lines_board (Board *board)
{
  for (int i = 0; i < 5; i++)
    {
      int sumj = 0;
      for (int j = 0; j < 5; j++)
        {
          sumj += board->marked[i][j];
        }
      if (sumj == 5)
        return i;
    }
  return -1;
}
int
sum_columns_board (Board *board, int column)
{
  int sumj = 0;
  for (int i = 0; i < 5; i++)
    {
      sumj += board->numbers[i][column];
    }
  return sumj;
}
int
sum_lines_board (Board *board, int line)
{
  int sumj = 0;
  for (int i = 0; i < 5; i++)
    {
      sumj += board->numbers[line][i];
    }
  return sumj;
}
int
sum_unmarked_board (Board *board)
{
  int sum = 0;
  for (int i = 0; i < 25; i++)
    {
      if (*(&board->marked[0][0] + i) == 0)
        {
          sum += *(&board->numbers[0][0] + i);
        }
    }
  return sum;
}

int
verify_columns_board (Board *board)
{
  for (int i = 0; i < 5; i++)
    {
      int sumj = 0;
      for (int j = 0; j < 5; j++)
        {
          sumj += board->marked[j][i];
        }
      if (sumj == 5)
        return i;
    }
  return -1;
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
  char *bit, *token;
  char *saveptr;
  // get drawn numbers
  fscanf (file, "%ms", &bit);

  /* int numBoards=3; */
  int numBoards = 100;
  Board boards[numBoards];

  for (int i = 0; i < numBoards; i++)
    {
      initialize_board (&boards[i]);
      read_board (&boards[i], file);
    }
  fclose (file);

  // draw numbers
  for (char *str = bit;; str = NULL)
    {
      token = strtok_r (str, ",", &saveptr);
      if (token == NULL)
        break;
      for (int i = 0; i < numBoards; i++)
        {
          if (boards[i].won == 0)
            {
              mark_board (&boards[i], atoi (token));
            }
          int line = verify_lines_board (&boards[i]);
          if (!boards[i].won && line >= 0)
            {
              boards[i].won = 1;
              printf ("%d\n", atoi (token) * sum_unmarked_board (&boards[i]));
            }
          int column = verify_columns_board (&boards[i]);
          if (!boards[i].won && column >= 0)
            {
              boards[i].won = 1;
              printf ("%d\n", atoi (token) * sum_unmarked_board (&boards[i]));
            }
        }
    }

  return 0;
}
