#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    if(!argv[1]){
        printf("no file given\n");
        exit(EXIT_FAILURE);
    }

    FILE * file = fopen(argv[1], "r");
    char *line;
    while(fscanf(file, "%ms",&line)!=EOF)
    {
    }
    printf("HELLO AOC!\n");

    return 0;
}
