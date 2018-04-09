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
court_timer_t the_timer;

const char GAME_OVER_MESSAGE[27] = "GAME OVER! press q to exit";
const char TRY_AGAIN_MESSAGE[38] = "TRY AGAIN! press any key to continue.";

void court_init()
/*
 * initializes all of the court and timer's struct fields
 * then edges and score of the court are drawn
 */
{
    the_timer.time = 0;
    the_court.balls_remaining = BALLS_REMAINING;
    the_court.time_duration = 1;

    draw_court_edges();
    draw_court_score();
}

void draw_court_edges()
/*
 * starts by drawing the top and bottom walls
 * followed by the left wall. A little padding is included in the LEFT_EDGE
 * and RIGHT_EDGE variables.
 */
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
/*
 * draws the scoreboad with consists of the the balls remaining and the
 * game clock. Note that the game clock only shows minutes and seconds
 * since I don't know who will play a single game for an hour straight
 */
{ 
    int center_x = COLS/2;
    int center_y = LINES/2;
    center_x -= (sizeof(TRY_AGAIN_MESSAGE)/2);
 
    // erases the try again message
    mvprintw(center_y, center_x, "                                     ");
    mvprintw(
        TOP_ROW - 1, 
        LEFT_EDGE, 
        "Balls Remaining: %d", 
        the_court.balls_remaining
    );

    int seconds = (the_timer.time / 50) % 60;
    int minutes = (the_timer.time / 50) / 60;

    mvprintw(
        TOP_ROW - 1,
        RIGHT_EDGE - strlen("Time: 00:00 "),
        "Time: %02d:%02d", minutes, seconds
    );
}

int court_round_over()
/*
 * Prints either the GAME_OVER_MESSAGE or the TRY_AGAIN_MESSAGE depending on the
 * number of balls left in the game. If there are still balls left, then
 * paddle and the ball get reinitialized for the start of the next round.
 */
{
    int center_x = COLS/2;
    int center_y = LINES/2;
    set_ticker(0);

    if (the_court.balls_remaining == 0) { 
        center_x -= (sizeof(GAME_OVER_MESSAGE)/2);
        mvprintw(center_y, center_x, GAME_OVER_MESSAGE);
        return 1; 
    } 

    the_court.balls_remaining--; 

    center_x -= (sizeof(TRY_AGAIN_MESSAGE)/2);
    mvprintw(center_y, center_x, TRY_AGAIN_MESSAGE);
    getch();
    draw_court_score();
    paddle_erase();
    paddle_init();
    ball_init();
    return 0; 
}

void court_update_timer()
/*
 * increments the timer and redraws the court score.
 */
{
    the_timer.time++;
    draw_court_score();
}