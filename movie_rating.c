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
typedef struct {
    // int userId;
    int movieId;
    float rating;
    // long timeStamp;
    int viewers;
} movie_model;

int accessSharedMem(movie_model *movie_record, int n){
    movie_model *shm_main_movie_record;
    size_t numberElements = 1682;
    size_t segment_size = sizeof(movie_model) * numberElements;
    int shmid = shmget(SHM_KEY, segment_size , 0666 | IPC_CREAT);
    if(shmid < 0){
        perror("Fail access shared memory");
        return 1;
    }
    else{
        printf("Accessed Shared memory: %d \n", shmid);
    }
    shm_main_movie_record = (movie_model*)shmat(shmid,NULL,0);

    if(shm_main_movie_record == (void *) - 1){
        perror("Fail to attach share memory");
        exit(-1);
    }
    int record_count = 0;
    while(record_count < n){
        movie_model movie = movie_record[record_count];
        int k = 0;
        while(shm_main_movie_record[k].rating != 0){
            if(shm_main_movie_record[k].movieId == movie.movieId){
                shm_main_movie_record[k].rating += movie.rating;
                break;
            }
            k++;
        }  
        if(shm_main_movie_record[k].rating == 0){
            shm_main_movie_record[k] = movie;
        }
        record_count++;
    }
    
    shmdt(shm_main_movie_record);
}
float movieCalRating(int i){
    printf("child doing ===================\n");
    FILE *fptr;
    movie_model movie_record[2000];
    if(i == 0)
        fptr = fopen("movie-100k_1.txt","r");
    else    
        fptr = fopen("movie-100k_2.txt","r");
    if(fptr == NULL){
        printf("Cannot open file");
    }

    char *line= NULL;
    size_t len = 0;
    size_t read = 0;
    int movieCount = 0;

    while((read = getline(&line, &len, fptr)) != -1){
        int duplicate = 0;
        char* token = strtok(line, " \t");
        int count = 0;
        movie_model new_movie ;
        new_movie.viewers=1;
        while(token != NULL){
            if(count == 1){
                new_movie.movieId = atoi(token);
            }
            if(count == 2){
                new_movie.rating =  atof(token);
            }
            count++;
            token = strtok(NULL, " \t");
        }
        for(int j = 0 ; j < movieCount;j++){
            if(movie_record[j].movieId == new_movie.movieId){
                movie_record[j].rating += new_movie.rating;
                movie_record[j].viewers++;
                duplicate = 1;
            }
        }
        if(duplicate == 0)
            movie_record[movieCount++] = new_movie;
    }
    
    fclose(fptr);
    if(line) free(line);
    accessSharedMem(movie_record, movieCount);
}
void deleteMem(){
    size_t numberElements = 1682;
    size_t segment_size = sizeof(movie_model) * numberElements;
    int shmid = shmget(SHM_KEY, segment_size , 0666 | IPC_CREAT);
    if(shmid < 0){
        perror("Failed access shared memory");
        return;
    }
    else{
        printf("Accessed Shared memory: %d \n", shmid);
    }
    if(shmctl(shmid, IPC_RMID, 0) == -1){
        perror("shmtcl error");
        return;
    }
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
    printf("Parent execute\n");
    // read from share mem
    size_t numberElements = 1682;
    size_t segment_size = sizeof( movie_model) * numberElements;

    int shmid = shmget(SHM_KEY, segment_size, 0644 | IPC_CREAT);

    movie_model* shm_main_movie_record;
    if(shmid < 0){
        perror("Fail access shared memory");
        return 1;
    }
    else{
        printf("Accessed Shared memory: %d \n", shmid);
    }
    shm_main_movie_record = ( movie_model*)shmat(shmid, 0 ,0);

    if(shm_main_movie_record == ( movie_model*) - 1){
        perror("Fail to attach share memory");
        exit(-1);
    }
    for(int i = 0;i < 1682;i++){
        printf("movie %d: .. id: %d.. rating: %f .. viewers: %d .. average rating: %f \n",i, shm_main_movie_record[i].movieId, shm_main_movie_record[i].rating, shm_main_movie_record[i].viewers, shm_main_movie_record[i].rating/shm_main_movie_record[i].viewers);
    }
    shmdt(shm_main_movie_record);

    deleteMem();
    return 0;
}
