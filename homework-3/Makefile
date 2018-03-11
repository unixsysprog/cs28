CC = cc
CFLAGS = -D _GNU_SOURCE -Wall -std=c99

sttyl: main.c sttyl.c tables.c
	$(CC) $(CFLAGS) main.c sttyl.c tables.c -o sttyl

clean: 
	rm -f sttyl main.o sttyl.o tables.o