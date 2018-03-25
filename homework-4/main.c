#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "court.h"

#define	TOP_ROW		3		/* top row in bounds */
#define	BOT_ROW 	(LINES - 4)	/* bot row in bounds */
#define	LEFT_EDGE	3	
#define	RIGHT_EDGE	(COLS - 4)

/** the main loop **/
void set_up();
void wrap_up();
// void placech(int, int, char);
// void placestr(int, int, char*);

int main()
{

	set_up(); 
    sleep(10);
	wrap_up();
	return 0;
}

/*	init ppball struct, signal handler, curses	*/

void set_up()
{
	void	ball_move(int);

    static court_t court = {
        .balls_remaining = 2,
        .origin = (point_t) {5, 5},
        .time_duration = 1
    };


	initscr();		/* turn on curses	*/
	noecho();		/* turn off echo	*/
	cbreak();		/* turn off buffering	*/

	draw_court(&court);
	// signal(SIGINT, SIG_IGN);	/* ignore SIGINT	*/
	refresh(); 
}

/* stop ticker and curses */
void wrap_up()
{ 
	endwin();		/* put back to normal	*/
}
