#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 1000
typedef struct BOARD {
    int numbers[BOARD_SIZE][BOARD_SIZE];
} Board;

void
initialize_board(Board *board)
{
    memset(&board->numbers[0][0], 0, BOARD_SIZE*BOARD_SIZE*sizeof(int));
}

void
print_board(Board board)
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if(!board.numbers[i][j])
            {
                printf(".\t");
            } else {
                printf("%d\t",board.numbers[i][j]);
            }
        }
        printf("\n");
    }
}

int
count_board_greater(Board * board,int number)
{
    int count=0;
    for (int i=0; i < BOARD_SIZE*BOARD_SIZE; i++) {
        count+=(*(&board->numbers[0][0]+i)>=number)?1:0;
    }

    return count;
}
int
main(int argc, char *argv[])
{
    if(!argv[1]){
        printf("no file given\n");
        exit(EXIT_FAILURE);
    }


    FILE * file = fopen(argv[1], "r");
    int x1,y1;
    int x2,y2;
    Board board;
    initialize_board(&board);
    while(fscanf(file, "%d,%d -> %d,%d",&x1,&y1,&x2,&y2)!=EOF)
    {
        if(x1==x2||y1==y2)
        {
            if(x2<x1||y2<y1){
                int temp=x1;
                x1=x2;
                x2=temp;
                temp = y1;
                y1=y2;
                y2=temp;
            }

            for(int i=x1;i<=x2;i++)
            {
                for(int j=y1;j<=y2;j++)
                {
                    board.numbers[j][i]+=1;
                }
            }
        }
    }
    printf("%d\n",count_board_greater(&board,2));


    return 0;
}
