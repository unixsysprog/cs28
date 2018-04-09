#include <curses.h>
#include <signal.h>
#include <stdlib.h>

#include "alarmlib.h"
#include "court.h"
#include "paddle.h"
#include "ball.h"

#define MAX(a, b) ((a) > (b) ? a : b)

void ball_move(int); 
bounce_states_t bounce_or_lose(ball_t *);

ball_t the_ball; 

void ball_init()
/* 
 * initializes the ball struct
 * sets the signal for ball_move and
 * sets the timer
 */
{ 
    the_ball.x_pos = 10;
    the_ball.y_pos = 10;
    the_ball.x_delay = the_ball.x_count = rand() % 10 + X_DELAY;
    the_ball.y_delay = the_ball.y_count = rand() % 10 + Y_DELAY;
    the_ball.x_dir = 1;
    the_ball.y_dir = 1;
    the_ball.symbol = DFL_SYMBOL;

    mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);

    signal( SIGALRM, ball_move );
    set_ticker( 1000 / TICKS_PER_SECOND );
	refresh();
}

void ball_move(int s)
/* 
 * updates the position of the ball after each alarm
 * if the ball leaves the court, then the function returns
 * otherwise it reinitializes the signal for the next round
 */
{
	int	y_cur, x_cur, moved;

	signal( SIGALRM , SIG_IGN );		/* dont get caught now 	*/
	court_update_timer();
	y_cur = the_ball.y_pos ;		/* old spot		*/
	x_cur = the_ball.x_pos ;
	moved = 0 ;

	if ( the_ball.y_delay > 0 && --the_ball.y_count == 0 ){
		the_ball.y_pos += the_ball.y_dir ;	/* move	*/
		the_ball.y_count = the_ball.y_delay  ;	/* reset*/
		moved = 1;
	}

	if ( the_ball.x_delay > 0 && --the_ball.x_count == 0 ){
		the_ball.x_pos += the_ball.x_dir ;	/* move	*/
		the_ball.x_count = the_ball.x_delay  ;	/* reset*/
		moved = 1;
	}

	if ( moved ){
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
		if (bounce_or_lose( &the_ball ) == lose) {
			court_round_over();
			return;
		}
		move(LINES-1, COLS-1);		/* park cursor	*/
		refresh();
	}
    
    signal(SIGALRM, ball_move);		/* re-enable handler	*/
}

bounce_states_t bounce_or_lose(ball_t *bp)
/* bounce_or_lose: if ball hits walls, change its direction
 *   args: address to ppball
 *   rets: 1 if a bounce happened, 0 if not
 */
{
	if ( bp->y_pos == TOP_BOUND ) { 
		bp->y_dir = 1 ;
		return bounce;
	}
	if ( bp->y_pos == BOT_BOUND ) {
		bp->y_dir = -1 ; 
		return bounce; 
	}

	if ( bp->x_pos == LEFT_BOUND ) {
		bp->x_dir = 1 ;
		return bounce; 
	}
	if ( bp->x_pos == RIGHT_BOUND && paddle_contact(bp)) { 
		bp->x_dir = -1 ;
		/* adjusts the speed usually faster but sometimes slower*/
		bp->x_delay = MAX(1, bp->x_delay + (rand() % 5 - 3)); 
		bp->y_delay = MAX(1, bp->y_delay + (rand() % 5 - 3));
		return bounce;
	}

	if (bp->x_pos > COLS) { /*the player should watch the ball move offscreen */
		return lose;
	}

	return in_progress;
}
