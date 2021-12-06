#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LanterfishSchool
{
    long lanterfish[9];

} LanterfishSchool;

void
init_school(LanterfishSchool * school)
{
    for (int i = 0; i < 9; i++) {
        school->lanterfish[i] = 0;
    }
}

void
add_fish_school(LanterfishSchool * school,int timer)
{
    school->lanterfish[timer]++;
}

void
update_school(LanterfishSchool * school)
{
    long fish[9];
    for (int i = 1; i < 9; i++) {
        fish[(i-1)]=school->lanterfish[i];
    }
    fish[8]=school->lanterfish[0];
    fish[6]+=school->lanterfish[0];
    for (int i = 0; i < 9; i++) {
        school->lanterfish[i]=fish[i];
    }

}

long
sum_school(LanterfishSchool school)
{
    long total=0;
    for (int i = 0; i < 9; i++) {
        total+=school.lanterfish[i];
    }
    return total;
}

void
print_school(LanterfishSchool school)
{
    for (int i = 0; i < 9; i++) {
        printf("%ld\t",school.lanterfish[i]);
    }
    printf("\033[31m%ld\t\033[0m",sum_school(school));
    printf("\n");
}


int
main(int argc, char *argv[])
{
    if(!argv[1]){
        printf("no file given\n");
        exit(EXIT_FAILURE);
    }


    FILE * file = fopen(argv[1], "r");
    char* line;

    fscanf(file, "%ms",&line);

    char * token;
    char * saveptr;
    LanterfishSchool school;
    init_school(&school);
    for (char * str = line; ; str=NULL) {
        token=strtok_r(str, ",", &saveptr);
        if(token==NULL) break;
        add_fish_school(&school, atoi(token));
    }
    /* printf("\t\033[33m0\t1\t2\t3\t4\t5\t6\t7\t8\033[31m\tTotal\n\033[0m"); */

    int day;
    for (day = 1; day < 257; day++) {
        update_school(&school);
    }

    printf("Total number of fishes: %ld\n",sum_school(school));

    return 0;
}
