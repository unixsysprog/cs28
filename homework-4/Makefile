CC = cc
CFLAGS = -D _GNU_SOURCE -Wall -std=c99

PREFIX = $(CC) $(CFLAGS)
pong: main.o alarmlib.o court.o ball.o paddle.o
	 $(PREFIX) main.o court.o alarmlib.o ball.o paddle.o -lcurses -o pong

main.o: main.c
	 $(PREFIX) -c main.c 

alarmlib.o: alarmlib.c *.h
		$(PREFIX) -c alarmlib.c

clean: 
	rm -f pong *.o

