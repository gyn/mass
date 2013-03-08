
CC = gcc

CFLAGS  = -Wall -g -ffunction-sections -fdata-sections
CFLAGS +=
LDFLAGS =

PROGRAM = mm

OJBS = main.o alloc.o

$(PROGRAM) : $(OJBS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OJBS)

clean:
	rm -rf $(PROGRAM) *.o
