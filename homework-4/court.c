#include <stdio.h>
#include <curses.h>
#include <string.h>

#include "alarmlib.h"
#include "court.h"
#include "ball.h"
#include "paddle.h"

void draw_court_edges();
void draw_court_score();

court_t the_court;
const char GAME_OVER_MESSAGE[27] = "GAME OVER! press q to exit";
const char TRY_AGAIN_MESSAGE[38] = "TRY AGAIN! press any key to continue.";

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
    int center_x = (LINES/2) - (sizeof(TRY_AGAIN_MESSAGE)/2);
    mvprintw(COLS/2, center_x, "                                     "); /* erases the try again message */
    mvprintw(TOP_ROW - 1, LEFT_EDGE, "Balls Remaining: %d", the_court.balls_remaining);
    mvprintw(TOP_ROW - 1, RIGHT_EDGE - strlen("Time: 00:00 "), "Time: 00:00");
}

int court_round_over()
{
    int center_x = LINES/2;
    set_ticker(0);
    if (the_court.balls_remaining == 0) { 
        center_x -= (sizeof(GAME_OVER_MESSAGE)/2);
        mvprintw(COLS/2, center_x, GAME_OVER_MESSAGE);
        return 1; 
    } 

    the_court.balls_remaining--; 

    center_x -= (sizeof(TRY_AGAIN_MESSAGE)/2);
    mvprintw(COLS/2, center_x, TRY_AGAIN_MESSAGE);
    getch();
    draw_court_score();
    paddle_init();
    ball_init();
    return 0; 
}