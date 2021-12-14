#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rule {
    int id;
    char name[3];
    char result1[3];
    char result2[3];
} Rule;

Rule
*
create_rule(int id,char name[3], char result[2])
{
    Rule * rule=malloc(sizeof(Rule));
    rule->id=id;
    for (int i = 0; i < 3; i++) {
        rule->name[i]=name[i];
    }
    rule->result1[0]=name[0];
    rule->result1[1]=result[0];
    rule->result2[0]=result[0];
    rule->result2[1]=name[1];
    return rule;
}

int
get_rule_id(Rule * rules,int nrules,char * ruleName)
{
    for (int i=0; i < nrules; i++) {
        if(strcmp(rules[i].name,ruleName)==0) return rules[i].id;
    }
    return -1;
}

void
update_population(unsigned long * population,Rule * rules,int nrules)
{
    long newPopulation[nrules];
    for (int i = 0; i < nrules; i++) {
        newPopulation[i]=0;
    }

    for (int i = 0; i < nrules; i++) {
        newPopulation[get_rule_id(rules, nrules,(rules[i].result1))]+=*(population+i);
        newPopulation[get_rule_id(rules, nrules,(rules[i].result2))]+=*(population+i);
    }

    for (int i = 0; i < nrules; i++) {
        *(population+i)=newPopulation[i];
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
    char *template;
    fscanf(file, "%ms",&template);

    char *pair;
    char *resulting;
    Rule * rules;
    int maxrules=120;
    rules=malloc(maxrules*sizeof(Rule));
    int nrules=0;

    unsigned long * pair_population;
    pair_population=malloc(maxrules*sizeof(long));
    memset(pair_population,0,maxrules*sizeof(long));

    unsigned long population[26] = {};

    while(fscanf(file, "%ms",&pair)!=EOF)
    {
        getc(file); getc(file); getc(file);
        fscanf(file, "%ms",&resulting);

        *(rules+nrules)=*create_rule(nrules, pair, resulting);
        free(pair);
        free(resulting);
        nrules++;
    }

    char code[3] = "  \0";
    char * saveptr=template;
    int ret;
    while((ret=sscanf(saveptr, "%2s",code)))
    {
        pair_population[get_rule_id(rules, nrules, code)]+=1;
        saveptr++;
        if(saveptr>=template+strlen(template)-1) break;
    }

    for (int i = 0; i < 10; i++) {
        update_population(pair_population,rules,nrules);
    }

    for (int i = 0; i < nrules; i++) {
        char c1 = rules[i].name[0];
        char c2 = rules[i].name[1];
        population[c1-65]+=pair_population[i];
        population[c2-65]+=pair_population[i];
    }

    unsigned long maximum=0;
    unsigned long minimum=-1;
    for (int i = 0; i < 26; i++) {
        population[i]=(population[i]+1)/2;
        if(population[i]>maximum) maximum=population[i];
        if(population[i]<minimum&&population[i]!=0) minimum=population[i];
    }

    printf("%ld\n",maximum-minimum);

    free(template);
    return 0;
}
