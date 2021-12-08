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

    int count=0;
    while(fscanf(file, "%m[^|]s",&line)!=EOF)
    {
        getc(file);
        fscanf(file, "%m[^\n]s",&line);
        if(line)
        {
            char * s;
            char * saveptr;
            saveptr=line;
            for (int i = 0; i < 4; i++) {
                sscanf(saveptr, "%ms",&s);
                switch (strlen(s)) {
                    case 2: // #1
                    case 3: // #7
                    case 4: // #4
                    case 7: // #8
                        count++;
                        break;
                    default:
                        break;
                }
                saveptr+=strlen(s)+1;
                free(s);
            }
        }
        free(line);
    }

    printf("%d\n",count);
    return 0;
}
