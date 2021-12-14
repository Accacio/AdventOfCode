#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rule {
    int id;
    char name[3];
    char result1[3];
    char result2[3];
} Rule;

typedef struct PolymerGen {
    Rule * rules;
    int nrules;
} PolymerGen;

Rule
create_rule(int id,char name[3], char result[2])
{

}



int
main(int argc, char *argv[])
{
    if(!argv[1]){
        printf("no file given\n");
        exit(EXIT_FAILURE);
    }

    FILE * file = fopen(argv[1], "r");
    char *template;
    fscanf(file, "%ms",&template);
    printf("%s\n",template);

    char *pair;
    char *resulting;
    while(fscanf(file, "%ms",&pair)!=EOF)
    {
        getc(file); getc(file); getc(file);
        fscanf(file, "%ms",&resulting);
        printf("%s %s\n",pair,resulting);

        free(pair);
        free(resulting);
    }


    free(template);
    return 0;
}
