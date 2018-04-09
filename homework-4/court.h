#ifndef BALLS_REMAINING

#define BALLS_REMAINING 2 

#define	TOP_ROW		3
#define	BOT_ROW 	LINES - 3
#define	LEFT_EDGE   3
#define	RIGHT_EDGE	COLS - 10

#define TOP_BOUND   ( TOP_ROW + 1 )
#define BOT_BOUND   ( BOT_ROW - 1 )
#define LEFT_BOUND  ( LEFT_EDGE + 1 )
#define RIGHT_BOUND ( RIGHT_EDGE - 1 ) 

typedef struct court {
    int balls_remaining;
    int time_duration;
} court_t; 

typedef struct court_timer {
    int time;
} court_timer_t;

void court_init();
int court_round_over();
void court_update_timer();

#endif