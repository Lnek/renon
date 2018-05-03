
CC=gcc
CFLAGS=-g -Wall -Werror

BINARY=ren

ALL_O=main.o ren_epoll.o ren_event.o ren_malloc.o ren_net.o ren_server.o ren_minheap.o

all:$(ALL_O) $(BINARY)

ALL_O:
		$(CC) $< -o $@ -c $(CFLAGS)

$(BINARY):$(ALL_O)
		$(CC) $(ALL_O) -o $(BINARY) $(CFLAGS)

clean:
		rm $(BINARY) $(ALL_O)

main.o: main.c ren_event.h ren_event.c ren_server.h ren_server.c
ren_epoll.o: ren_epoll.c
ren_event.o: ren_event.c ren_event.h ren_epoll.c ren_malloc.h ren_malloc.c
ren_malloc.o: ren_malloc.h ren_malloc.c
ren_net.o: ren_net.h ren_net.c
ren_server.o: ren_server.h ren_server.c ren_event.h ren_event.c 
ren_min_heap.o: ren_min_heap.c ren_min_heap.h
