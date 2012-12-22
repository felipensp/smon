CC?=gcc
CFLAGS=-g -Wall -pedantic
OBJECTS=ptrace.o smon.o main.o

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
smon: $(OBJECTS)
	$(CC) -osmon $(OBJECTS)
	
clean:
	rm -rf smon *.o
