CC := gcc
CFLAGS :=  -I.
DEPS := imp.h
OBJ := calc.o imp.o 
all: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY:clean
clean:
	rm -f all *.o
