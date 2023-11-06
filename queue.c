#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>

/* Remember to initilize the queue before using it */
void initialize_queue(struct pqueue_t * q) {
	q->head = q->tail = NULL;
	pthread_mutex_init(&q->lock, NULL);
}

/* Return non-zero if the queue is empty */
int empty(struct pqueue_t * q) {
	return (q->head == NULL);
}

/* Get PCB of a process from the queue (q).
 * Return NULL if the queue is empty */
struct pcb_t * de_queue(struct pqueue_t * q) {
	struct pcb_t * proc = NULL;
	// TODO: return q->head->data and remember to update the queue's head
	// and tail if necessary. Remember to use 'lock' to avoid race
	// condition
	
	// YOUR CODE HERE
	pthread_mutex_lock(&(q->lock));
	if(empty(q)) {
		pthread_mutex_unlock(&(q->lock));
		return proc;
	}
	proc = q->head->data;
	struct qitem_t*temp = q->head;
	if(q->head->next == NULL){
		q->head = NULL;
		q->tail= NULL;
	}
	else{
		q->head = q->head->next;
		free(temp);
	}
	pthread_mutex_unlock(&(q->lock));
	return proc;
}
struct pcb_t * get_proc_by_priority(struct pqueue_t* q,int timestamp){
	pthread_mutex_lock(&(q->lock));
	struct pcb_t * proc = NULL;
	if(empty(q)) {
		pthread_mutex_unlock(&(q->lock));
		return NULL;
	}
	int priority = -1;
	int arr_time = 0;
	struct qitem_t * travel = q->head;
	struct qitem_t * res = NULL;
	while(travel != NULL){
		if(travel->data->arrival_time <= timestamp){
			if(priority == -1)
			{
				priority = travel->data->priority;
				proc = travel->data;
				res = travel;
				arr_time = travel->data->arrival_time;
			} 
			else{
				if(travel->data->priority < priority) {
					priority = travel->data->priority;
					proc = travel->data;
					res = travel;
					arr_time = travel->data->arrival_time;
				}
				else if(travel->data->priority == priority){
					if(travel->data->arrival_time < arr_time){
						priority = travel->data->priority;
						proc = travel->data;
						res = travel;
						arr_time = travel->data->arrival_time;
					}
				}
			}
		}
		travel = travel->next;
	}
	struct pcb_t* temp = q->head->data;
	q->head->data = res->data;
	res->data = temp;

	pthread_mutex_unlock(&(q->lock));
	return de_queue(q);
}
struct pcb_t* get_next_proc_by_priority(struct pqueue_t* q, int priority){
	struct pcb_t * proc = NULL;
	pthread_mutex_lock(&(q->lock));
	if(empty(q)) {
		pthread_mutex_unlock(&(q->lock));
		return proc;
	}
	struct qitem_t * travel = q->head;
	while(travel != NULL){
		if(travel->data->priority <= priority){
			proc = travel->data;
			break;
		}
		travel = travel->next;
	}
	pthread_mutex_unlock(&(q->lock));
	return proc;
}
void en_queue_head(struct pqueue_t * q, struct pcb_t * proc){
	pthread_mutex_lock(&(q->lock));
	if(empty(q)){
		q->head = (struct qitem_t *)malloc(sizeof(struct qitem_t));
		q->head->data = proc;
		q->head->next = NULL;
		q->tail = q->head;
	}
	else{
		struct qitem_t* dumb = (struct qitem_t *)malloc(sizeof(struct qitem_t));
		dumb->data = proc;
		dumb->next = q->head;
		q->head = dumb;
	}
	pthread_mutex_unlock(&(q->lock));
}
/* Put PCB of a process to the queue. */
void en_queue(struct pqueue_t * q, struct pcb_t * proc) {
	// TODO: Update q->tail.
	// Remember to use 'lock' to avoid race condition
	
	// YOUR CODE HERE
	pthread_mutex_lock(&(q->lock));
	if(empty(q)){
		q->head = (struct qitem_t *)malloc(sizeof(struct qitem_t));
		q->head->data = proc;
		q->head->next = NULL;
		q->tail = q->head;
	}
	else{
		struct qitem_t* dumb = (struct qitem_t *)malloc(sizeof(struct qitem_t));
		dumb->data = proc;
		dumb->next = NULL;
		q->tail->next = dumb;
		q->tail = q->tail->next;
	}
	pthread_mutex_unlock(&(q->lock));
}


