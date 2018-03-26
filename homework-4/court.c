#include <stdio.h>
#include <curses.h>
#include <string.h>

#include "court.h"

void draw_court_edges();
void draw_court_score();

court_t the_court;

void court_init()
{
    the_court.balls_remaining = BALLS_REMAINING;
    the_court.time_duration = 1;

    draw_court_edges();
    draw_court_score();
}

void draw_court_edges()
{
    // drawing horizontal walls
    for (int x = LEFT_EDGE; x < RIGHT_EDGE; x++ ) { 
        mvaddch(TOP_ROW, x, '=');
        mvaddch(BOT_ROW, x, '=');
    }

    // drawing the vertical left wall
    for (int y = TOP_ROW; y < BOT_ROW; y++) {
        mvaddch(y, LEFT_EDGE, '|');
    }
}

void draw_court_score()
{ 
    mvprintw(TOP_ROW - 1, LEFT_EDGE, "Balls Remaining: %d", the_court.balls_remaining);
    mvprintw(TOP_ROW - 1, RIGHT_EDGE - strlen("Time: 00:00 "), "Time: 00:00");
}
