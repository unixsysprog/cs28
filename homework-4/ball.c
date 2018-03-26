#include <curses.h>
#include <signal.h>

#include "alarmlib.h"
#include "court.h"
#include "ball.h"

void ball_move(int); 
bounce_states_t bounce_or_lose(ball_t *);

ball_t the_ball; 

void ball_init()
{ 
    the_ball.x_pos = 10;
    the_ball.y_pos = 10;
    the_ball.x_delay = the_ball.x_count = X_DELAY;
    the_ball.y_delay = the_ball.y_count = Y_DELAY;
    the_ball.x_dir = 1;
    the_ball.y_dir = 1;
    the_ball.symbol = DFL_SYMBOL;

    mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);

    signal( SIGALRM, ball_move );
    set_ticker( 1000 / TICKS_PER_SECOND );
}

void ball_move(int s)
{
	int	y_cur, x_cur, moved;

	signal( SIGALRM , SIG_IGN );		/* dont get caught now 	*/
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
		bounce_or_lose( &the_ball );
		move(LINES-1, COLS-1);		/* park cursor	*/
		refresh();
	}
    
    signal(SIGALRM, ball_move);		/* re-enable handler	*/
}

/* bounce_or_lose: if ball hits walls, change its direction
 *   args: address to ppball
 *   rets: 1 if a bounce happened, 0 if not
 */
bounce_states_t bounce_or_lose(ball_t *bp)
{
	bounce_states_t return_val = 0 ;

	if ( bp->y_pos == TOP_BOUND )
		bp->y_dir = 1 , return_val = bounce ;
	else if ( bp->y_pos == BOT_BOUND )
		bp->y_dir = -1 , return_val = bounce;

	if ( bp->x_pos == LEFT_BOUND )
		bp->x_dir = 1 , return_val = bounce ;
	else if ( bp->x_pos == RIGHT_BOUND )
		bp->x_dir = -1 , return_val = bounce;

	return return_val;
}
