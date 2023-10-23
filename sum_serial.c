#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<pthread.h>
#include "global.h"

int main(int argc, char *argv[])
{

    if( argc != 2){
        fprintf(stderr, "usage a.out");
        return -1;
    }
    if(atoi(argv[1]) < 0){
        fprintf(stderr,"%d must be >= 0 \n", atoi(argv[1]));
        return -1;
    }
    clock_t start_time, end_time;
    start_time = clock();
    long upper = atoi(argv[1]);
    long long int sum = 0 ;
    for(int i = 1;i <= upper;i++){
        sum += i;
    }
    printf("%lld \n", sum );
    end_time = clock();
    printf("Run time: %f \n", ((double)(end_time - start_time) / CLOCKS_PER_SEC));
    return 0;
}
