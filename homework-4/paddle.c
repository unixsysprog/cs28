#include <curses.h>

#include "court.h"
#include "ball.h" 
#include "paddle.h"

void draw_paddle();
void paddle_erase();

paddle_t the_paddle;

void paddle_init()
/* 
 * sets the initial values of the paddle struct and then makes a draw call
 * 
 */
{
    the_paddle.x_pos = PAD_START_X;
    the_paddle.y_pos = PAD_START_Y;
    the_paddle.length = PAD_LENGTH;
    the_paddle.symbol = PADDLE_SYMBOL; 

    draw_paddle();
}

void draw_paddle() 
/*
 * draws 5 X's vertically starting from the paddles origin (upper left corner)
 */
{
    for (int i = 0; i < the_paddle.length; i++) {
        mvaddch(the_paddle.y_pos + i, the_paddle.x_pos, the_paddle.symbol);
    } 
}

void paddle_erase() 
/*
 * erases the paddle by replacing the current set of X's with blank spaces
 */
{
    for (int i = 0; i < the_paddle.length; i++) {
        mvaddch(the_paddle.y_pos + i, the_paddle.x_pos, BLANK_PADDLE);
    } 
}

void paddle_move(dir_t dir)
/*
 * changes the paddles direction. If the paddle is already at the top of the 
 * bottom of the screen then just update the cursor but don't update the 
 * paddle's position
 * 
 * if the paddle is in the middle of the court, then it gets erased, it's 
 * position is updated, and then redrawn
 */
{
    if (the_paddle.y_pos < TOP_BOUND && dir == up) {
        move(LINES - 1, COLS - 1);  /* prevent cursor from jumping */
        return;
    }
    if (the_paddle.y_pos + the_paddle.length > BOT_ROW && dir == down) {
        move(LINES - 1, COLS - 1);  /* prevent cursor from jumping */
        return; 
    } 

    paddle_erase();
    the_paddle.y_pos += dir;
    draw_paddle();
    move(LINES - 1, COLS - 1);  /* prevent cursor from jumping */
}

int paddle_contact(ball_t *ball)
/*
 * this function only checks to see if the ball is within the height of the
 * paddle. In the logic in the bounce_or_lose function determines whether the 
 * ball is actually at the right edge of the court.
 * 
 */
{
    return (
        the_paddle.y_pos < ball->y_pos && 
        ball->y_pos < the_paddle.y_pos + the_paddle.length
    );
}