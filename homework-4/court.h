#include "point.h"

typedef struct court {
    int balls_remaining;
    point_t origin;
    int time_duration;
} court_t; 

void draw_court(court_t *);
void draw_court_edges(court_t *);