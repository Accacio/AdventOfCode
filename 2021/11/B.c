#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct map {
    int * map;
    int rows;
    int cols;
} map;

void
print_map(map map)
{
    for (int i = 0; i < map.rows; i++) {
        for (int j = 0; j < map.cols; j++) {
            int * cur = (map.map+j+i*map.cols);
            /* if(*cur==0) */
            printf("\033[%dm",32-(*cur));
            printf("%d\033[0m", *(map.map+j+i*map.cols));
        }
        printf("\n");
    }
}

void
init_map(map * map,int rows, int cols)
{
    map->cols=cols;
    map->rows=rows;
    map->map = malloc(map->cols*map->rows*sizeof(int));
    memset(map->map, 0, map->cols*map->rows*sizeof(int));
}

void
sum_value_map(map * map,int value)
{
    for (int i = 0; i < map->rows*map->cols; i++) {
            *(map->map+i)+=value;
    }
}

long
propagate_flash_util(map * map,int row,int col)
{
    long ret = 0;
    if(row>=0&&row<map->rows&&col>=0&&col<map->cols){
        int * cur = (map->map+col+row*map->cols);
        if(*cur!=0){
            (*cur)++;
            if(*cur>9){
                ret = 1;
                *cur=0;
                ret+=propagate_flash_util(map,row-1,col-1);
                ret+=propagate_flash_util(map,row-1,col);
                ret+=propagate_flash_util(map,row-1,col+1);

                ret+=propagate_flash_util(map,row,col-1);
                ret+=propagate_flash_util(map,row,col+1);

                ret+=propagate_flash_util(map,row+1,col-1);
                ret+=propagate_flash_util(map,row+1,col);
                ret+=propagate_flash_util(map,row+1,col+1);
            }
        }
    }
    return ret;
}

long
propagate_flash(map * map)
{
    long ret=0;
    for (int i = 0; i < map->rows*map->cols; i++) {
        int * cur = (map->map+i);
        if(*cur>9){
            ret+=propagate_flash_util(map, i/map->cols, i%map->cols);
        }
    }
    return ret;
}

int
main(int argc, char *argv[])
{
    if(!argv[1]){
        printf("no file given\n");
        exit(EXIT_FAILURE);
    }

    map map;
    init_map(&map, 10, 10);

    FILE * file = fopen(argv[1], "r");
    char *line;
    int count=0;
    while(fscanf(file, "%ms",&line)!=EOF)
    {
        char c[2]=" \0";
        char *saveptr=line;
        do {
            sscanf(saveptr,"%c",c);
            *(map.map+count)=atoi(c);
            saveptr++;
            count++;
        } while (saveptr!=line+strlen(line));
    }

    long totalFlashes=0;
    int * zeroMap=malloc(10*10*sizeof(int));
    memset(zeroMap, 0, 10*10*sizeof(int));

    int it=1;
    while (1) {
        printf("\033[2J\033[1;1H");
        sum_value_map(&map,1);
        long flashes=propagate_flash(&map);
        int cmp=memcmp(map.map, zeroMap, 10*10*sizeof(int))==0;
        print_map(map);
        printf("\n");
        usleep(12*1e4/3);
        if(cmp) break;
        it++;
    }
    printf("%d\n", it);

    return 0;
}
