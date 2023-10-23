#include<stdio.h>
#include<string.h>

int main(){
    char line[] = "Phong      cute       phomai   ";
    char * token = strtok(line, " ");
    while(token != NULL){
        printf("%s-\n", token);
        token = strtok(NULL, " ");
    }
    return 0;

}