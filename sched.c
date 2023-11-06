
#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_UNIT	100 // In microsecond

static struct pqueue_t in_queue; // Queue for incomming processes
static struct pqueue_t ready_queue; // Queue for ready processes

static int load_done = 0;
int num_proc = 0;
static int timeslot; 	// The maximum amount of time a process is allowed
			// to be run on CPU before being swapped out

// Emulate the CPU
void * cpu(void * arg) {
	int timestamp = 0;
	int time = 0;
	/* Keep running until we have loaded all process from the input file
	 * and there is no process in ready queue */
	while (!load_done || !empty(&ready_queue)) {
		/* Pickup the process from the queue */
		struct pcb_t * proc = get_proc_by_priority(&ready_queue,timestamp);

		// struct pcb_t * proc = de_queue(&ready_queue);
		if (proc == NULL) {
			/* If there is no process in the queue then we
			 * wait until the next time slice */
			timestamp++;
			usleep(TIME_UNIT);
		}else{
			printf("----\n");
			struct pcb_t * next_proc = get_next_proc_by_priority(&ready_queue, proc->priority);
			// if(next_proc != NULL)
			// 	printf("proc : %d, next proc: %d", proc->pid, next_proc->pid);
			// else{
			// 	printf("proc : %d, next proc: null", proc->pid);
			// }
			// printf("\n");
			/* Execute the process */
			int start = timestamp; 	// Save timestamp
			int id = proc->pid;	// and PID for tracking
			int exec_time = 0;

			// TODO: Calculate exec_time from process's PCB
			
			// YOUR CODE HERE
			if(next_proc == NULL){
				exec_time = proc->burst_time;
			}
			else{
				if(proc->priority == next_proc->priority){
					exec_time = proc->burst_time;
				}
				else if(proc->priority < next_proc->priority){
					exec_time = next_proc->arrival_time-timestamp;
				}
			}
			// exec_time = proc->burst_time;
			/* Emulate the execution of the process by using
			 * 'usleep()' function */
			usleep(exec_time * TIME_UNIT);
			
			/* Update the timestamp */
			timestamp += exec_time;

			// TODO: Check if the process has terminated (i.e. its
			// burst time is zero. If so, free its PCB. Otherwise,
			// put its PCB back to the queue.

			proc->burst_time -= exec_time;
			// YOUR CODE HERE
			if(proc->burst_time <= 0){
				free(proc);
			}
			else{
				if(proc->priority > 0)
					proc->priority--;
				en_queue_head(&ready_queue, proc);
			}
			/* Track runtime status */
			printf("%2d-%2d: Execute %d\n", start, timestamp, id);
		}
	}
	return NULL;
}

// Emulate the loader
void * loader(void * arg) {
	int timestamp = 0;
	/* Keep loading new process until the in_queue is empty*/
	while (!empty(&in_queue)) {
		struct pcb_t * proc = de_queue(&in_queue);
		/* Loader sleeps until the next process available */
		int wastetime = proc->arrival_time - timestamp;
		usleep(wastetime * TIME_UNIT);
		/* Update timestamp and put the new process to ready queue */
		timestamp += wastetime;
		en_queue(&ready_queue, proc);
		// printf("%d\n", ready_queue.tail->data->arrival_time);
	}
	/* We have no process to load */
	load_done = 1;
	return NULL;
}

/* Read the list of process to be executed from stdin */
void load_task() {
	scanf("%d %d\n", &timeslot, &num_proc);
	int i;
	for (i = 0; i < num_proc; i++) {
		struct pcb_t * proc =
			(struct pcb_t *)malloc(sizeof(struct pcb_t));
		scanf("%d %d %d\n", &proc->arrival_time, &proc->burst_time,&proc->priority);
		proc->pid = i;
		en_queue(&in_queue, proc);
	}
}

int main() {
	pthread_t cpu_id;	// CPU ID
	pthread_t loader_id;	// LOADER ID

	/* Initialize queues */
	initialize_queue(&in_queue);
	initialize_queue(&ready_queue);

	/* Read a list of jobs to be run */
	load_task();
	/* Start cpu */
	pthread_create(&cpu_id, NULL, cpu, NULL);
	/* Start loader */
	pthread_create(&loader_id, NULL, loader, NULL);

	/* Wait for cpu and loader */
	pthread_join(cpu_id, NULL);
	pthread_join(loader_id, NULL);

	pthread_exit(NULL);

}


