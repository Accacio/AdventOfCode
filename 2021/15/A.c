#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Map {
    int *data;
    int *visited;
    int nmap;
    int nmax;
    int cols;
    int rows;
} Map;

void
init_map(Map * map,int nmax)
{
    map->nmax=2*nmax;
    map->cols=nmax;
    map->rows=1;
    map->nmap=0;
    map->data = malloc(map->nmax*sizeof(int));
    memset(map->data, 0, map->nmax*sizeof(int));
    map->visited = malloc(map->nmax*sizeof(int));
    memset(map->visited, 0, map->nmax*sizeof(int));
}

void
map_add_value(Map * map, int value)
{
    if(map->nmap==map->nmax) {
       map->data = realloc(map->data, 2*map->nmax*sizeof(int));
       memset(map->data+map->nmax, 0, map->nmax*sizeof(int));
       map->visited = realloc(map->visited, 2*map->nmax*sizeof(int));
       memset(map->visited+map->nmax, 0, map->nmax*sizeof(int));
       map->nmax*=2;
    }
    *(map->data+map->nmap)=value;
    map->nmap++;
    map->rows=map->nmap/map->cols;
}

void
print_map(Map * map)
{
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            printf("%d", *(map->data+j+i*map->rows));
        }
        printf("\n");
    }
}

static
void
print_visited(Map * map)
{
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            printf("%d", *(map->visited+j+i*map->rows));
        }
        printf("\n");
    }
}


unsigned
long
find_path_util(Map * map, unsigned long currentTotal, unsigned long currentMinimum,int cur,int end)
{
    unsigned long ret=0;
    int curx=cur%map->cols;
    int cury=cur/map->cols;
    printf("(%d,%d) %d\n",curx,cury,*(map->data+cur));
    /* if(ret>minimum) */
    /* printf("%d\n", *(map->data+cur)); */
    *(map->visited+cur)=1;
    if(cur==end) {
        *(map->visited+end)=0;
        printf("Found a path\n");
        return currentTotal;
    }
    /* print_visited(map); */
    /* printf("\n"); */

    unsigned long subtotalmin=-1;

    for (int i=1; i>=0; i--) {
        for (int j = 1; j >= 0; j--) {
            if(i!=j&&i!=-j) {
                int nextx=curx+i;
                int nexty=cury+j;
                if(nextx>=0&&nextx<map->cols&&nexty>=0&&nexty<map->rows){
                    int next=nexty*map->cols+nextx;
                    int nextvalue=*(map->data+next);
                    if(next<map->nmap && nextvalue+currentTotal < currentMinimum && *(map->visited+next)!=1) {
                        /* printf("visit\n"); */
                        unsigned long tot=find_path_util(map, currentTotal, currentMinimum, next, end);
                        subtotalmin = nextvalue+tot<subtotalmin ? nextvalue+tot : subtotalmin;
                        /* printf("visited found %ld %ld %d\n",tot,subtotalmin,tot<=subtotalmin); */
                    }
                }
            }
        }
    }

    *(map->visited+cur)=0;

    /* printf("%ld ",subtotalmin); */


    return subtotalmin+currentTotal;

}

/* long */
/* graph_search_paths(Graph * graph, int initial, int final, int visitedTwiceAlready) */
/* { */
/*     long ret= 0; */
/*     Node * cur=graph->nodes+initial; */
/*     cur->timesVisited++; */

/*     if(cur->numId==final) { */
/*         cur->timesVisited--; */
/*         return 1; */
/*     } */

/*     for (int i=0; i < cur->totalEdges; i++) { */
/*         Node * next = graph->nodes+cur->edges[i]; */
/*         int visited=visitedTwiceAlready; */
/*         if(next->numId==0||((!next->big && next->timesVisited>0)&&visitedTwiceAlready)) { */
/*             continue; */
/*         } */
/*         if(!next->big && next->timesVisited>0&&!visitedTwiceAlready) { */
/*             visited=1; */
/*         } */
/*         ret+=graph_search_paths(graph, next->numId, final,visited); */
/*     } */
/*     cur->timesVisited--; */
/*     return ret; */
/* } */

unsigned
long
find_path(Map * map)
{
    unsigned long ret=-1;
    ret=find_path_util(map, 0, ret,0, map->nmap-1);
    return ret;
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
    /* print_map(&map); */
    printf("Total = %d\n\n",map.nmap);

    printf("%lu\n", find_path(&map));


    return 0;
}
