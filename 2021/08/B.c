#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
print_binary(char * string)
{
    char chars[]= "abcdefg";
    for (int i = 0; i < sizeof(chars)/sizeof(char)-1; i++) {
        char c[2]=" \0";
        c[0]=chars[i];
        if(strpbrk(string, c)){
            printf("1");
        } else{
            printf("0");
        }
    }
}

int
get_representation(char * string)
{
    int result = 0;
    char chars[]= "abcdefg";
    for (int i = 0; i < sizeof(chars)/sizeof(char)-1; i++) {
        char c[2]=" \0";
        c[0]=chars[i];
        if(strpbrk(string, c)){
            result+=1<<(7-i);
        } else{
        }
    }
    return result;
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
    char numString[5] = "____";

    /* printf("Number %s\n",numString); */
    long count=0;
    char * nums[10];
    for (int i = 0; i < 10; i++) {
        nums[i]=NULL;
    }
    while(fscanf(file, "%m[^|]s",&line)!=EOF)
    {
        /* clear string */
        for (int i = 0; i < 4; i++) {
            numString[i] = '_';
        }

        char * s;
        char * saveptr;
        saveptr=line;

        for (int i = 0; i < 10; i++) {
            sscanf(saveptr, "%ms",&s);
            if(s){
            /* printf("%s\n",s); */
            switch (strlen(s)) {
                case 2:
                    nums[1]=s;
                    break;
                case 3:
                    nums[7]=s;
                    break;
                case 4:
                    nums[4]=s;
                    break;
                case 7: // #8
                    nums[8]=s;
                    break;
                default:
                    break;
            }
            saveptr+=strlen(s)+1;
            }
        }

        getc(file); // get pipe

        fscanf(file, "%m[^\n]s",&line);
        if(line)
        {
            saveptr=line;

            for (int i = 0; i < 4; i++) {
                sscanf(saveptr, "%ms",&s);
                int matching = 0;
                switch (strlen(s)) {
                    case 2: // #1
                        numString[i]='1';
                        break;
                    case 3: // #7
                        numString[i]='7';
                        break;
                    case 4: // #4
                        numString[i]='4';
                        break;
                    case 7: // #8
                        numString[i]='8';
                        break;
                    case 5: // #5 #2 #3
                        for (int i = 0; i < strlen(nums[1]); i++) {
                            if(strchr(s,nums[1][i])) matching++;
                        }
                        if(matching==strlen(nums[1])) numString[i]='3';
                        else {
                            matching = 0;
                            for (int i = 0; i < strlen(nums[4]); i++) {
                                if(strchr(s,nums[4][i])) matching++;
                            }
                            if(matching==3){
                                numString[i]='5';
                            } else{
                                numString[i]='2';
                            }
                        }
                        break;
                    case 6: // #0 #6 #9
                        for (int i = 0; i < strlen(nums[1]); i++) {
                            if(strchr(s,nums[1][i])) matching++;
                        }
                        if(matching!=strlen(nums[1])) numString[i]='6';
                        else {
                            matching = 0;
                            for (int i = 0; i < strlen(nums[4]); i++) {
                                if(strchr(s,nums[4][i])) matching++;
                            }
                            if(matching==4){
                                numString[i]='9';
                            } else{
                                numString[i]='0';
                            }
                        }
                        break;
                    default:
                        break;
                }
                saveptr+=strlen(s)+1;
                free(s);
            }
        }
        count+=strtol(numString, NULL, 10);
        free(line);
    }

    printf("%ld\n",count);
    return 0;
}
