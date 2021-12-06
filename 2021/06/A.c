#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Lanternfish
{
    int timer;
    int alive;
} Lanterfish;

void
init_lanterfish(Lanterfish * lanternfish, int timer,int alive)
{
    lanternfish->timer=timer;
    lanternfish->alive=alive;
}


typedef struct LanterfishSchool
{
    Lanterfish * lanterfish;
    int numLanternfish;

} LanterfishSchool;

void
init_school(LanterfishSchool * school)
{
    school->lanterfish = NULL;
    school->numLanternfish = 0;
}

void
school_add_fish(LanterfishSchool * school,Lanterfish * lanterfish)
{
    school->numLanternfish++;
    school->lanterfish = realloc(school->lanterfish, school->numLanternfish*sizeof(Lanterfish));
    *(school->lanterfish+school->numLanternfish-1)=*lanterfish;
}

void
print_school(LanterfishSchool school)
{
    for (int i=0; i < school.numLanternfish-1 ; i++) {
        printf("%d,",(school.lanterfish+i)->timer);
    }
        printf("%d\n",(school.lanterfish+school.numLanternfish-1)->timer);
}

void
update_school(LanterfishSchool * school)
{
    for (int i=0; i < school->numLanternfish; i++) {
        Lanterfish *current=(school->lanterfish+i);
        if(current->alive)
        {
        if(current->timer>0)
        {
            current->timer--;
        } else {

            current->timer=6;
            Lanterfish fish;
            init_lanterfish(&fish, 8,0);
            school_add_fish(school, &fish);
        }} else {
            current->alive=1;
        }
    }
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
        Lanterfish afish;
        init_lanterfish(&afish, atoi(token),1);
        school_add_fish(&school, &afish);
    }

    for (int days = 1; days <= 80; days++) {
        update_school(&school);
    }
    printf("Total number of fishes: %d",school.numLanternfish);

    return 0;
}
