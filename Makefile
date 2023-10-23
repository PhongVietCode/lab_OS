CC=gcc
CFLAGS=-I.
TARGET=practise reader sum_serial sum_multithread
all: sum_serial sum_multithread
practise:
	$(CC) practise.c -o $@  $(CFLAGS)
	./$@
reader:
	$(CC) reader.c  -o $@ $(CFLAGS)
	./$@

sum_serial:
	$(CC) sum_serial.c -o $@  $(CFLAGS)
	./$@ 100000000
sum_multithread: 
	$(CC) sum_multithread.c -o $@  $(CFLAGS) -lpthread
	./$@ 10 100000000
clean:
	rm -f $(TARGET)