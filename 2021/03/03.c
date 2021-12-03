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
    int bit_columns=12;
    char bit[bit_columns+1];
    char gamma_bits[bit_columns+1];
    char epsilon_bits[bit_columns+1];
    int ones[bit_columns];
    int totalNums = 0;
    memset(ones, 0, sizeof(ones));

    FILE * file = fopen(argv[1], "r");
    while(fscanf(file, "%s",bit)!=EOF)
    {
        for (int i = 0; i < bit_columns; i++) {
            ones[i]+=strtol(bit,NULL,2)&1<<(bit_columns-1-i)?1:0;
        }
        totalNums++;

    }
    fclose(file);

    for (int i = 0; i < bit_columns; i++) {
        sprintf(&gamma_bits[i], "%d",ones[i]>(totalNums/2)?1:0);
        sprintf(&epsilon_bits[i], "%d",ones[i]>(totalNums/2)?0:1);
    }
    printf("%ld\n",strtol(gamma_bits,NULL,2)*strtol(epsilon_bits,NULL,2));
    return 0;
}
