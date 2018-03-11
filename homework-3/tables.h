/* 
 * Definitions for Reads
 */

struct flaginfo { tcflag_t fl_value; char *fl_name; };
void show_baud(speed_t);
void show_some_flags( struct termios *);
void show_flagset( int thevalue, struct flaginfo[] );

typedef enum CtrlFlags { 
    C_IFLAG,
    C_OFLAG,
    C_CFLAG,
    C_LFLAG,
    C_CC,
    NO_FLAG 
} c_flags_t;


/* 
 * Definitions for Writes
 */
typedef struct flag_description {
    char *key;
    int value;
    c_flags_t flag;
} flag_desc_t;

flag_desc_t lookup(char *flag_name);
void turn_bit_on(struct termios *, flag_desc_t *, char *);
void turn_bit_off(struct termios *, flag_desc_t *, char *);
void set_control(struct termios *, flag_desc_t *, int, char **, int);

