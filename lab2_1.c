#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<pthread.h>
#include<wait.h>
#include<string.h>
#include "global.h"
struct movie_model{
    // int userId;
    int movieId;
    float rating;
    // long timeStamp;
};
void accessSharedMem(){
    printf("Acces Shared Mem\n");
}
void addToRecord(struct movie_model* movie_record, int n, struct movie_model model, int* i){
    for(int j =0 ;j < n;j++){
        if(movie_record[j].movieId == model.movieId){
            movie_record[j].rating += model.rating;
            return;
        }
    }
    movie_record[++(*i)] = model;
}
float movieCalRating(int i){
    printf("child doing\n");
    FILE *fptr;
    char *line = NULL;
    size_t len = 0;
    size_t read;
    struct movie_model movie_record[2000];
    if(i == 0)
        fptr = fopen("movie-100k_1.txt","r");
    else    
        fptr = fopen("movie-100k_2.txt","r");
    if(fptr == NULL){
        printf("Cannot open file");
    }
    int i = 0;
    while((read = getline(&line,&len, fptr)) != -1){
        // printf("%s", line);
        /* extract rating from movie */
        struct movie_model model;
        char *token = strtok(line, " ");
        int count = 0;
        while(token != NULL){
            if(count == 1){
                model.movieId = atoi(token);
            }
            else if(count == 2){
                model.rating = atoi(token);
            }
            count++;
        }
        addToRecord(movie_record,2000,model,i);
    }

    fclose(fptr);
    if(line) free(line);

    accessSharedMem();
}
int main(int argc, char *argv[])
{
    /* code */
    pid_t child_pid, wpid;
    int status = 0;
    for(int i = 0 ;i < CHILD_PROCESS;i++){
        if((child_pid = fork()) == 0){
            // child code
            movieCalRating(i);
            exit(0);
        }
    }
    // parent code
    while((wpid = wait(&status)) > 0);
    printf("Parent code\n");
    return 0;
}
