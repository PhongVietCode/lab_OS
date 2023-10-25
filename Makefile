CC=gcc
CLFAGS=-I.
TARGET=sched
all: 
	${CC} sched.c queue.c -o sched ${CLFAGS} -lpthread
clean: 
	rm -f sched