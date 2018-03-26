CC = cc
CFLAGS = -D _GNU_SOURCE -Wall -std=c99

pong: main.c alarmlib.c court.c ball.c paddle.c
	$(CC) $(CFLAGS) main.c court.c alarmlib.c ball.c paddle.c -lcurses -o pong

clean: 
	rm -f pong main.o alarmlib.o