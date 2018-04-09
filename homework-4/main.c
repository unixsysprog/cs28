#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "court.h"
#include "ball.h"
#include "alarmlib.h"
#include "paddle.h"

int set_up();
void wrap_up();

/** the main loop **/
int main()
{ 
	if (set_up() == 0) {
		return 0;
	}

	char c;
	while ((c = getch()) != 'q') {
		/* user vim keys to move paddle */
		if (c == 'k') { 
			paddle_move(up);
			continue;
		} 
		if (c == 'j') {
			paddle_move(down);
			continue; 
		}
	}
	wrap_up();
	return 0;
}

/*	init ppball struct, signal handler, curses	*/ 
int set_up()
{ 
	initscr();		/* turn on curses	*/
	noecho();		/* turn off echo	*/
	cbreak();		/* turn off buffering	*/
	if (LINES < 15 || COLS < 50) {
		fprintf(stderr, "Screen is too small!\n");
		wrap_up();
		return 0;
	}

	srand(getpid());
	court_init();
	paddle_init();
	ball_init();
	// signal(SIGINT, SIG_IGN);	/* ignore SIGINT	*/
	refresh(); 

	return 1;
}

/* stop ticker and curses */
void wrap_up()
{ 
	set_ticker(0);
	endwin();		/* put back to normal	*/
}
