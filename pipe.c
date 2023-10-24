#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
static int pipefd1[2], pipefd2[2];
// PIPEFD1 : PARENT WRITE - CHILD READ
// PIPEFD2 : PARENT READ -  CHILD WRITE
void init(void){
    if(pipe(pipefd1) < 0 || pipe(pipefd2) < 0){
        perror("pipe create failed");
        exit(EXIT_FAILURE);
    }
}
void WRITE_TO_PARENT(void){
    //TODO
    close(pipefd2[0]);
    char message[] = "Hello dady !!!";
    write(pipefd2[1],message, sizeof(message));
    printf("Child message to parent: %s \n", message);
}
void READ_FROM_PARENT(void){
    //TODO
    close(pipefd1[1]);
    char read_message[128];
    read(pipefd1[0], read_message, sizeof(read_message));
    printf("Child read message from parent: %s \n", read_message);
}
void WRITE_TO_CHILD(void){
    //TODO
    close(pipefd1[0]);
    char message[] = "Hello my son !!!";
    write(pipefd1[1],message, sizeof(message));
    printf("Parent write message to child: %s \n", message);
}
void READ_FROM_CHILD(void){
    //TODO
    close(pipefd2[1]);
    char read_message[128];
    read(pipefd2[0], read_message, sizeof(read_message));
    printf("Parent read message from child: %s \n", read_message);
}

int main(int argc, char* argv[]){
    init();
    pid_t pid;
    pid = fork();
    alarm(10);
    if(pid == 0){
        // parent       
         while(1){
            sleep(rand()%2 + 1);
            WRITE_TO_CHILD();
            READ_FROM_CHILD();
        }
    }
    else{
        // child
         while(1){
            sleep(rand()%2 + 1);
            WRITE_TO_PARENT();
            READ_FROM_PARENT();
        }
        exit(0);
    }
    return 0;
}