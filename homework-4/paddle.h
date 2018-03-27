#include "ball.h"
#include "court.h"

#define PAD_START_X ( RIGHT_EDGE )
#define PAD_START_Y ( (TOP_ROW + BOT_ROW) / 2 )
#define PAD_LENGTH 5
#define PADDLE_SYMBOL 'X'
#define BLANK_PADDLE ' '

typedef struct paddle {
    int x_pos, y_pos, length;
    char symbol;
} paddle_t;

typedef enum direction {
    up = -1, down = 1
} dir_t;

void paddle_init();
void paddle_move();
int paddle_contact(ball_t *);