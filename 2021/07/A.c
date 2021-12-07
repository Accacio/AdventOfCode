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
    fscanf(file, "%ms",&line);

    int * numbers;
    numbers = (int*) malloc(strlen(line)*sizeof(int));
    memset(numbers, 0, strlen(line));


    char *saveptr;
    char * token;
    int count=0;
    for(char * str = line;;str=NULL,count++){
        token = strtok_r(str, ",", &saveptr);
        if (!token) break;
        numbers[count]=atoi(token);
    }
    numbers = realloc(numbers, count*sizeof(int));

    long minimum;
    long minimumidx;
    for (int i = 0; i < count; i++) {
        long sum=0;
        for (int j=0; j < count; j++) {
            if(j!=i){
                sum+=abs(numbers[i]-numbers[j]);
                if(sum>=minimum){
                    break;
                }
            }
        }
        if(sum<minimum){
            minimum=sum;
            minimumidx=i;
        }
    }
    printf("Fuel: \033[34m%ld\033[0m | Index: %ld\n",minimum,minimumidx);

    return 0;
}
