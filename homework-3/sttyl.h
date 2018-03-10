#include <termios.h>

enum CtrlFlags { C_IFLAG, C_OFLAG, C_CFLAG, C_LFLAG, C_CC, NO_FLAG };
typedef enum CtrlFlags c_flags_t;

void show_baud(speed_t);
void show_some_flags( struct termios *);
void read_settings(struct termios*, struct winsize*);
void write_settings(struct termios*, int, char **);