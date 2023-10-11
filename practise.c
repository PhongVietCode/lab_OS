#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<pthread.h>
#include "global.h"
int glo_init_data = 99;
int glo_noninint_data;

void print_func(){
    int local_data = 9;
    printf("Process id: %d \n", getpid());
    printf("Address of the process: \n");
    printf("1. glo_init_data: %p \n", &glo_init_data);
    printf("2. glo_noninint_data %p \n", &glo_noninint_data);
    printf("3. print_func(): %p \n", &print_func);
    printf("4. local_data: %p \n", &local_data);
}

int shmcreate(){
    int shmid;
    char* shm;
    printf("key: %d \n", SHM_KEY);
    // create shared memory segment
    shmid = shmget(SHM_KEY,1000,IPC_CREAT | 0644);
    if(shmid < 0){
        perror("Share memory");
        return 1;
    }
    else{
        printf("[writer] Shared memory id: %d \n", shmid);
    }
    // attach memory
    shm = (char *)shmat(shmid,0,0);
    if(shm ==  (char*) - 1){
        perror("shmat");
        exit(1);
    }
    // write to shared memory segment
    sprintf(shm,"hello world \n");
    printf("[writer] share memory: %p \n",shm);
    printf("[writer] shared memory content: %s \n", shm);

    // DELETE shm
    if(shmdt(shm) == -1){
        perror("shmdt");
        return 1;
    }
    
    return 0;
}

int sum;
void *runner(void *param);

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;
    if( argc != 2){
        fprintf(stderr, "usage a.out");
        return -1;
    }
    if(atoi(argv[1]) < 0){
        fprintf(stderr,"%d must be >= 0 \n", atoi(argv[1]));
        return -1;
    }
    /* get default attribute for attr */
    pthread_attr_init(&attr);
    /* create pthread and pass argv*/
    pthread_create(&tid, &attr,runner,argv[1]);
    /* wait the thread to exit */
    pthread_join(tid,NULL);
    printf("sum = %d \n", sum);
    return 0;
}
void *runner(void *param){
    int i, upper = atoi(param);
    sum = 0;
    for(int i= 0;i <= upper;i++){
        sum += i;
    }
    pthread_exit(0);
}