#include <curses.h>

#include "court.h"
#include "ball.h" 
#include "paddle.h"

void draw_paddle();
void erase_paddle();

paddle_t the_paddle;

void paddle_init()
{
    if (the_paddle.symbol != '\0')
        erase_paddle();                 /* clears paddle after a lose */

    the_paddle.x_pos = PAD_START_X;
    the_paddle.y_pos = PAD_START_Y;
    the_paddle.length = PAD_LENGTH;
    the_paddle.symbol = PADDLE_SYMBOL; 

    draw_paddle();
}

void draw_paddle() {
    for (int i = 0; i < the_paddle.length; i++) {
        mvaddch(the_paddle.y_pos + i, the_paddle.x_pos, the_paddle.symbol);
    } 
}

void erase_paddle() {
    for (int i = 0; i < the_paddle.length; i++) {
        mvaddch(the_paddle.y_pos + i, the_paddle.x_pos, BLANK_PADDLE);
    } 
}

void paddle_move(dir_t dir)
{
    if (the_paddle.y_pos < TOP_BOUND && dir == up)
        return;
    if (the_paddle.y_pos + the_paddle.length > BOT_ROW && dir == down)
        return;

    erase_paddle();
    the_paddle.y_pos += dir;
    draw_paddle();
    move(LINES - 1, COLS - 1);  /* prevent cursor from jumping */
}

int paddle_contact(ball_t *ball)
{
    return (
        the_paddle.y_pos < ball->y_pos && 
        ball->y_pos < the_paddle.y_pos + the_paddle.length
    );
}