PORT=31337

CFLAGS= -DPORT=$(PORT) -g -Wall 

all: main_sync test

main_sync : main_sync.o procs.o socket.o structs.o
	gcc ${CFLAGS} -o $@ $^

test :  test.o procs.o structs.o
	gcc ${CFLAGS} -o $@ $^

%.o : %.c
	gcc ${CFLAGS} -c $<

clean: 
	rm *.o