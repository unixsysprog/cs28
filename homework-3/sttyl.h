
void show_baud(speed_t);
void show_some_flags( struct termios *);
void read_settings(struct termios*, struct winsize*);
void write_settings(struct termios*, int, char **);