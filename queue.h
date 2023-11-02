
#ifndef QUEUE_H
#define QUEUE_H

#include "structs.h"

/* Initialize the process queue */
void initialize_queue(struct pqueue_t * q);

/* Get a process from a queue */
struct pcb_t * de_queue(struct pqueue_t * q);
struct pcb_t * get_proc_by_priority(struct pqueue_t *q, int timestamp);
struct pcb_t* get_next_proc_by_priority(struct pqueue_t* q, int priority);
/* Put a process into a queue */
void en_queue(struct pqueue_t * q, struct pcb_t * proc);
void en_queue_head(struct pqueue_t * q, struct pcb_t * proc);

int empty(struct pqueue_t * q);

#endif


