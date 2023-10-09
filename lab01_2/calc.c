#include "imp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
    char exitStr[] = "exit";
    system("clear");
    while (1)
    {
        char input[256];
        printf(">> ");
        fgets(input,sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';
        if(strcmp(input, exitStr) == 0){
            break;
        }
        calc(input, sizeof(input));
        printf("Press any key to continue...");
        getchar();
        system("clear");
    }
}