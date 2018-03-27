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

void set_up();
void wrap_up();

/** the main loop **/
int main()
{ 
	set_up(); 
	char c;
	while ((c = getch()) != 'q') {
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
void set_up()
{ 
	initscr();		/* turn on curses	*/
	noecho();		/* turn off echo	*/
	cbreak();		/* turn off buffering	*/

	srand(getpid());
	court_init();
	paddle_init();
	ball_init();
	// signal(SIGINT, SIG_IGN);	/* ignore SIGINT	*/
	refresh(); 
}

/* stop ticker and curses */
void wrap_up()
{ 
	set_ticker(0);
	endwin();		/* put back to normal	*/
}
