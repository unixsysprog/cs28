#include <curses.h>

#include "court.h"

#ifndef BLANK

#define	BLANK      ' '
#define DFL_SYMBOL 'o'
#define X_DELAY 5
#define Y_DELAY 8

#define TICKS_PER_SECOND 50

typedef struct ppball {
    int x_pos;
    int x_dir;
    int x_delay;
    int x_count; 

    int y_pos; 
    int y_dir; 
    int y_delay; 
    int y_count; 

    char symbol;
} ball_t;

typedef enum bounce_states { 
    lose, 
    bounce,
    in_progress
} bounce_states_t;

void ball_init();

#endif 