#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Map {
    int *data;
    long *costs;
    int nmap;
    int nmax;
    int cols;
    int rows;
    int costcols;
    int costrows;
} Map;

typedef struct queue {
    long * data;
    long max;
    long start;
    long size;
} queue;

void
init_queue(queue * queue, long max)
{
    queue->max = max;
    queue->start = 0;
    queue->size = 0;
    queue->data = malloc(2*max*sizeof(long));
    memset(queue->data, 0, 2*max*sizeof(long));
}

void
enqueue(queue * queue, long i, long j)
{
    long place;
    if(queue->size < queue->max) {
        place = (queue->start+queue->size)%queue->max;
        *(queue->data+place*2)=i;
        *(queue->data+place*2+1)=j;
        queue->size++;
    }
}

void
dequeue(queue * queue, long * i, long * j)
{
    if(queue->size > 0) {
        *i = *(queue->data+queue->start*2);
        *j = *(queue->data+queue->start*2+1);
        queue->start = (queue->start+1)%queue->max;
        queue->size--;
    }
}


void
init_map(Map * map,int nmax)
{
    map->nmax=2*nmax;
    map->cols=nmax;
    map->rows=2;
    map->costcols=5*map->cols;
    map->costrows=5*map->rows;
    map->nmap=0;
    map->data = malloc(map->nmax*sizeof(int));
    memset(map->data, 0, map->nmax*sizeof(int));
    map->costs = malloc(map->costcols*map->costrows*sizeof(long));
    memset(map->costs, 1, map->costcols*map->costrows*sizeof(long));
}

void
map_add_value(Map * map, int value)
{
    if(map->nmap==map->nmax) {
        map->data = realloc(map->data, 2*map->nmax*sizeof(int));
        memset(map->data+map->nmax, 0, map->nmax*sizeof(int));
        map->costs = realloc(map->costs, 5*5*2*map->nmax*sizeof(long));
        memset(map->costs, 1, 5*5*2*map->nmax*sizeof(long));
        map->nmax*=2;
    }
    *(map->data+map->nmap)=value;
    map->nmap++;
    map->rows=map->nmap/map->cols;
    map->costrows=5*map->rows;
}

long
get_big_map(Map * map, long i, long j)
{
    return (*(map->data+(i%map->cols)+(j%map->rows)*map->cols)+i/map->cols+j/map->rows-1)%9+1;
}

void
print_map(Map * map)
{
    for (int i = 0; i < map->costrows; i++) {
        for (int j = 0; j < map->costcols; j++) {
            printf("%ld", get_big_map(map, j, i));
        }
        printf("\n");
    }
}

void
print_costs(Map * map)
{
    for (int i = 0; i < map->costrows; i++) {
        for (int j = 0; j < map->costcols; j++) {
            printf("%lu ", *(map->costs+j+i*map->costrows));
        }
        printf("\n");
    }
}


unsigned
long
find_path(Map * map)
{

    int neighbours[4][2] = {{-1, 0},
                            {0, -1},
                            {1, 0},
                            {0, 1}};
    queue queue;
    init_queue(&queue,100000);

    *(map->costs) = 0;
    enqueue(&queue, 0, 0);

    long ci,cj;
    while (queue.size>0) {
        dequeue(&queue , &ci, &cj);
        for (int i = 0; i < 4; i++) {
            long neigi = ci+neighbours[i][0];
            long neigj = cj+neighbours[i][1];
            if(neigi>=0&&neigj>=0&&neigi<map->costcols&&neigj<map->costrows)
            {
                long *dist = (map->costs+neigi+neigj*map->costcols);
                long alt = *(map->costs+ci+cj*map->costcols) + get_big_map(map,neigi,neigj);
                if(alt<*dist)
                {
                    *dist = alt;
                    enqueue(&queue, neigi, neigj); // should enqueue using sort
                }
            }

        }
    }
    return *(map->costs+map->costcols-1+(map->costrows-1)*map->costcols);
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

    Map map;

    fscanf(file, "%ms",&line);
    init_map(&map, strlen(line));

    char c[2]=" \0";
    char *saveptr=line;
    do {
        sscanf(saveptr,"%c",c);
        map_add_value(&map,atoi(c));
        saveptr++;
    } while (saveptr!=line+strlen(line));
    free(line);


    while(fscanf(file, "%ms",&line)!=EOF)
    {
        saveptr=line;
        do {
            sscanf(saveptr,"%c",c);
            map_add_value(&map,atoi(c));
            saveptr++;
        } while (saveptr!=line+strlen(line));
        free(line);
    }

    printf("%lu\n", find_path(&map));

    return 0;
}
