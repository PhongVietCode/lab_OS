#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>
#include<string.h>
#include "global.h"
long long sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
typedef struct{
    int thread_id;
    long long lower;
    long long upper;
    long long localSum;
} ThreadModel;
void *runner(void *thread_data);

int main(int argc, char *argv[])
{
    if( argc != 3){
        fprintf(stderr, "usage a.out");
        return -1;
    }
    if(atoi(argv[1]) < 0){
        fprintf(stderr,"%d must be >= 0 \n", atoi(argv[1]));
        return -1;
    }
    long long num_thread = atoi(argv[1]);
    long long max_number = atoi(argv[2]);
    int rc;
    pthread_t threads [num_thread];
    ThreadModel thread_data[num_thread];
    clock_t start_time, end_time;
    start_time = clock();
    for(int t = 0; t < num_thread;t++){
        thread_data[t].thread_id = t;
        thread_data[t].lower = t * (max_number / num_thread) + 1;
        thread_data[t].upper = (t + 1) * (max_number / num_thread);
        rc = pthread_create(&threads[t], NULL, runner,(void*) &thread_data[t]);
        if(rc){
            perror("Thread create fail");
            exit(EXIT_FAILURE);
        }
    }
    for(int i =0 ;i < num_thread;i++){
        pthread_join(threads[i], NULL);
        // sum+= thread_data[i].localSum;
    }
    printf("%lld \n", sum);
    end_time = clock();
    printf("Run time: %f \n", ((double)(end_time - start_time) / CLOCKS_PER_SEC));
    return 0;

}
void *runner(void *thread_data){
    ThreadModel *data =  (ThreadModel*) thread_data;
    data->localSum = 0;
    for(long long i = data->lower; i <= data->upper;i++){
        data->localSum += i;
    }
    // pthread_mutex_lock(&mutex);
    sum += data->localSum;
    // pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}