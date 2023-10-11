CC=gcc
CFLAGS=-I.
TARGET=practise reader
all: practise reader
practise:
	$(CC) practise.c -o $@  $(CFLAGS)
	./$@
reader:
	$(CC) reader.c  -o $@ $(CFLAGS)
	./$@
clean:
	rm -f $(TARGET)