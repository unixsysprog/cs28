CC = cc
CFLAGS = -D _GNU_SOURCE -Wall -std=c99

sttyl: main.c sttyl.c
	$(CC) $(CFLAGS) main.c sttyl.c -o sttyl

clean: 
	rm -f main.o sttyl.o