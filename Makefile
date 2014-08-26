CC = gcc
CFLAGS = -c -Iinclude -w
LFLAGS = -pthread 

all: serv1

serv1:  concurrent_hashmap.o log.o login_parser.o serv1.o
	$(CC) $(LFLAGS) concurrent_hashmap.o log.o login_parser.o serv1.o -o bin/serv1 -s

serv1.o:  include/concurrent_hashmap.h include/login_parser.h src/serv1.c
	$(CC) $(CFLAGS) src/serv1.c 

concurrent_hashmap.o: include/concurrent_hashmap.h src/concurrent_hashmap.c 
	$(CC) $(CFLAGS) src/concurrent_hashmap.c

login_parser.o: include/login_parser.h src/login_parser.c
	$(CC) $(CFLAGS) src/login_parser.c

log.o: include/concurrent_hashmap.h include/log.h  src/log.c
	$(CC) $(CFLAGS) src/log.c


