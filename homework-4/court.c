#include <stdio.h>
#include <curses.h>
#include <string.h>
#include "court.h"

void draw_court(court_t *court)
{
    draw_court_edges(court);
//    draw_court_score(court);
}

void draw_court_edges(court_t * court)
{
    // drawing horizontal walls
    for (int x = court->origin.x; x < COLS; x++ ) { 
        mvaddch(court->origin.y, x, '-');
        mvaddch(LINES - 1, x, '-');
    }

    // drawing the vertical wall
    for (int y = court->origin.y; y < LINES; y++) {
        mvaddch(y, court->origin.x, '|');
    }
}
