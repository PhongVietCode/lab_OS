#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include "global.h"

int readshm(){
    int shmid;
    char *shm;
    printf("key: %d \n", SHM_KEY);
    shmid = shmget(SHM_KEY,1000, IPC_CREAT | 0644);

    if(shmid < 0){
        perror("shmget");
        return 1;
    }
    else{
        printf("[reader] shared memory id: %d \n", shmid);
    }
    shm = (char *)shmat(shmid, 0,0);
    if(shm == (char *) - 1){
        perror("shmat");
        return 1;
    }
    printf("[reader] Shared memory %p \n",shm);
    if(shm != 0){
        printf("[reader] Shared memory content: %s \n", shm);
    }
    if(shmdt(shm) == -1){
        perror("shmdt");
        return 1;
    }
    // CONTROL operation
    if(shmctl(shmid,IPC_RMID, 0 ) == -1){
        perror("shmctl");
        return 1;
    }
    return 0;
}
int main(int argc, char const *argv[])
{
    readshm();
    return 0;
}
