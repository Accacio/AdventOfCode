#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Paper {
    int *map;
    int rows;
    int cols;
} Paper;


void
init_paper(Paper * paper,int rows, int cols)
{
    paper->rows = rows;
    paper->cols = cols;
    paper->map = (int*) malloc(rows*cols*sizeof(int));
    memset(paper->map,0,rows*cols*sizeof(int));
    /* for (int i = 0; i < rows; i++) { */
    /*     for (int j = 0; j < cols; j++) { */
    /*         *(paper->map+i+j*cols) = 0; */
    /*     } */
    /* } */
}

void
print_paper(Paper paper)
{
    for (int i = 0; i < paper.rows; i++) {
        for (int j = 0; j < paper.cols; j++) {
            if(*(paper.map+j+i*paper.cols))
            printf("#");
            else
            printf(".");
        }
        printf("\n");
    }
}

long
count_paper(Paper paper)
{
    long ret=0;
    for (int i = 0; i < paper.rows; i++) {
        for (int j = 0; j < paper.cols; j++) {
            int * cur=paper.map+j+i*paper.cols;
            ret+=*cur?1:0;
        }
    }
    return ret;
}


void
fold_paper_vert(Paper * paper,int row)
{
    long ret=0;
    for (int i = row; i < paper->rows; i++) {
        for (int j = 0; j < paper->cols; j++) {
            int * cur=paper->map+j+i*paper->cols;
            if(*cur){
            int corresponding_row=row-(i-row);
            *(paper->map+corresponding_row*paper->cols+j)=1;
            }
            /* printf("%d",*cur); */
        }
            /* printf("\n"); */
    }
    int oldrows=paper->rows;
    paper->rows = row;
    paper->map = realloc(paper->map,row*paper->cols*sizeof(int));
    /* memset(paper->map+oldrows*paper->cols,0,()*sizeof(int)); */
    return ret;
}

void
fold_paper_horz(Paper * paper,int col)
{
    long ret=0;
    int * init=paper->map;
    paper->map = malloc(col*paper->rows*sizeof(int));
    memset(paper->map,0,col*paper->rows*sizeof(int));

    for (int i = 0; i < paper->rows; i++) {
        for (int j = 0; j < col+1; j++) {
            int * cur=init+j+i*paper->cols;
            int * new=paper->map+j+i*col;
            *new=*cur;
        }
    }
    for (int i = 0; i < paper->rows; i++) {
        for (int j = col; j < paper->cols; j++) {
            int * cur=init+j+i*paper->cols;
            int * new=paper->map+j+i*paper->cols;
            if(*cur){
            int corresponding_col=col-(j-col);
            *(paper->map+corresponding_col+i*col)=1;
            }
            /* printf("%d",*cur); */
        }
            /* printf("\n"); */
    }
    paper->cols = col;
    free(init);
    return ret;
}


void
paper_add_value(Paper * paper,int x,int y)
{
    if(x>=paper->cols)
    {
        int * init=paper->map;
        paper->map = malloc((x+1)*paper->rows*sizeof(int));
        memset(paper->map,0,(x+1)*paper->rows*sizeof(int));

        for (int i = 0; i < paper->rows; i++) {
            for (int j = 0; j < paper->cols; j++) {
                int * cur=init+j+i*paper->cols;
                int * new=paper->map+j+i*(x+1);
                *new=*cur;
            }
        }
        paper->cols=x+1;
        free(init);
    }

    if(y>paper->rows)
    {
        paper->map=realloc(paper->map,(y+1)*paper->cols*sizeof(int));
        paper->rows=y+1;
    }
    *(paper->map+x+y*paper->cols) = 1;
}

int
main(int argc, char *argv[])
{
    if(!argv[1]){
        printf("no file given\n");
        exit(EXIT_FAILURE);
    }

    FILE * file = fopen(argv[1], "r");
    char *line;

    Paper paper;
    init_paper(&paper,1,1);
    /* init_paper(&paper,10,10); */

    /* print_paper(paper); */
    /* int x,y; */
    /* while(fscanf(file,"%d",&x)) */
    /* { */
    /*     getc(file); */
    /*     fscanf(file,"%d",&y); */
    /*     printf("%d %d\n",x,y); */
    /*     *(paper.map+x+y*paper.cols) = 1; */

    /* } */
    /* fscanf(file,"%d",&x) */
    /*             getc(file); */
    /*     fscanf(file,"%d",&y); */
    /*     printf("%d %d\n",x,y); */


    while(fscanf(file, "%ms",&line)&&strcmp(line,"fold")!=0)
    {

        /* if(!line) break; */
        int x;
        int y;
        char * saveptr;
        char * token = strtok_r(line, ",",&saveptr);
        x=atoi(token);
        token = strtok_r(NULL, ",",&saveptr);


        /* printf("%d %d\n",paper.rows, paper.cols); */
        /* token = strtok(NULL, ","); */
        y=atoi(token);
        /* free(line); */
        /* *(paper.map+x+y*paper.cols) = 1; */

        paper_add_value(&paper,x,y);
        printf("Size %d %d\n",paper.rows, paper.cols);
        printf("%d %d\n",x,y);
        free(line);
    }
    print_paper(paper);


        printf("\n");
    fold_paper_vert(&paper,7);
    fold_paper_horz(&paper,5);
        printf("\n");
        print_paper(paper);
    /* fold_paper_horz(&paper,5); */
    /* fscanf(file, "%ms",&line); */

        /* printf("\n"); */
        /* print_paper(paper); */
        printf("\n");
        printf("%ld\n",count_paper(paper));

    /* free(line); */

    return 0;
}
