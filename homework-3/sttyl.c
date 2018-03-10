#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include "sttyl.h" 

void read_settings(struct termios *ttyinfo, struct winsize *window)
{ 
    show_baud(cfgetospeed(ttyinfo));
    printf("rows %d; ", window->ws_row);
    printf("columns %d; ", window->ws_col);
    putchar('\n');
    printf("erase: Ctrl-%c\n", ttyinfo->c_cc[VERASE]-1+'A'); 
    printf("kill: Ctrl-%c\n", ttyinfo->c_cc[VKILL]-1+'A');

    show_some_flags( ttyinfo );                            /* show misc. flags	*/
} 

struct flag_desc {
    char *key;
    int value;
    c_flags_t flag;
};

struct flag_desc flag_list[] = {
    // Input Flags
    { .key = "igncr",  .value = IGNCR,  .flag = C_IFLAG },
    { .key = "iuclc",  .value = IUCLC,  .flag = C_IFLAG }, 
    { .key = "icrnl",  .value = ICRNL,  .flag = C_IFLAG },
    // Control Flags
    { .key = "hupcl",  .value = HUPCL,  .flag = C_CFLAG }, 
    // Local Flags
    { .key = "echo",   .value = ECHO,   .flag = C_LFLAG }, 
    { .key = "echoe",  .value = ECHOE,  .flag = C_LFLAG },
    { .key = "isig",   .value = ISIG,   .flag = C_LFLAG },
    { .key = "icanon", .value = ICANON, .flag = C_LFLAG },
    // Output Flags
    { .key = "opost",  .value = OPOST,  .flag = C_OFLAG }, 
    // Control Flags
    { .key = "erase",  .value = VERASE,  .flag = C_CC   }, 
    { .key = "kill",   .value = VKILL,   .flag = C_CC   }, 
    // Null terminating struct 
    { .key = NULL,     .value = -1,      .flag = NO_FLAG }  
};

int lookup(char *flag_name)
{
    int i;
    for( i = 0; flag_list[i].key != NULL; i++ ) {
        if (strcmp(flag_name, flag_list[i].key) == 0) {
            break;
        }
    }
    return i;
}

void set_control(struct termios *settings, struct flag_desc * flag_obj, int argc, char **argv, int idx)
{ 
    if (idx >= argc) {
        fprintf(stderr, "sttyl: missing argument to '%s'\n", flag_obj->key);
        exit(EXIT_FAILURE);
    }
    if (strlen(argv[idx]) != 1) {
        fprintf(stderr, "sttyl: invalid integer argument '%s'\n", argv[idx]);
        exit(EXIT_FAILURE);
    }
    settings->c_cc[flag_obj->value] = (int) argv[idx][0]; 
}

void turn_bit_off(struct termios *settings, struct flag_desc *flag_obj, char *arg)
{ 
    // turn the bit off for the correct flag
    switch (flag_obj->flag) {
        case C_IFLAG:
            settings->c_iflag &= ~flag_obj->value;
            break;
        case C_OFLAG:
            settings->c_oflag &= ~flag_obj->value;
            break;
        case C_CFLAG:
            settings->c_cflag &= ~flag_obj->value;
            break;
        case C_LFLAG:
            settings->c_lflag &= ~flag_obj->value;
            break;
        default:
            fprintf(stderr, "sttyl: invalid argument '%s'\n", arg);
            exit(EXIT_FAILURE); 
    } 
}

void turn_bit_on(struct termios *settings, struct flag_desc *flag_obj, char *arg)
{ 
    // turn the bit on for the correct flag
    switch (flag_obj->flag) {
        case C_IFLAG:
            settings->c_iflag |= flag_obj->value;
            break;
        case C_OFLAG:
            settings->c_oflag |= flag_obj->value;
            break;
        case C_CFLAG:
            settings->c_cflag |= flag_obj->value;
            break;
        case C_LFLAG:
            settings->c_lflag |= flag_obj->value;
            break;
        default:
            fprintf(stderr, "sttyl: invalid argument '%s'\n", arg);
            exit(EXIT_FAILURE); 
    } 
}

void write_settings(struct termios *settings, int argc, char **argv )
{
    int flag_idx;
    struct flag_desc flag_obj;
 
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {                 // check for negates
            flag_idx = lookup(&argv[i][1]);
            flag_obj = flag_list[flag_idx];

            turn_bit_off(settings, &flag_obj, argv[i]);
        } else {                                 // checking for positives 
            flag_idx = lookup(argv[i]);
            flag_obj = flag_list[flag_idx]; 

            if ( flag_obj.flag == C_CC ) {       // handle C_CC flags separately
                set_control(settings, &flag_obj, argc, argv, ++i); 
            } else {
                turn_bit_on(settings, &flag_obj, argv[i]);
            }
        }
    } 

    if ( tcsetattr(0, TCSANOW, settings ) != 0 ) {
        perror("sttyl");
        exit(EXIT_FAILURE);
    }
}

void show_baud(speed_t baud_speed)
{
    printf("speed "); 
    switch (baud_speed) {
        case B0:     printf("0");       break;
        case B50:    printf("50");      break;
        case B75:    printf("75");      break;
        case B110:   printf("110");     break;
        case B134:   printf("134");     break;
        case B150:   printf("150");     break;
        case B200:   printf("200");     break;
        case B300: 	 printf("300");     break;
        case B600:   printf("600");     break;
        case B1200:	 printf("1200");    break;
        case B1800:	 printf("1800");    break;
        case B2400:	 printf("2400");    break;
        case B4800:	 printf("4800");    break;
        case B9600:	 printf("9600");    break;
        case B19200: printf("19200");   break;
        case B38400: printf("38400");   break;
        default:     printf("Fast?");   break;
    }
    printf(" baud; "); 
} 

//[iflag, oflag, cflag, lflag, ispeed, ospeed, cc]
struct flaginfo { tcflag_t fl_value; char *fl_name; };

struct flaginfo input_flags[] = { 
    { IGNBRK,	"Ignore the break condition" 	    },
    { BRKINT,	"Signal interrupt on break"         },
    { IGNPAR,	"Ignore chars with parity errors"   },
    { PARMRK,	"Mark parity errors" 		        },
    { INPCK	,	"Enable input parity check"	        },
    { ISTRIP,	"Strip character" 		            },
    { INLCR	,	"Map NL to CR on input"             },
    { IGNCR	,	"Ignore CR" 				        },
    { ICRNL	,	"Map CR to NL on input" 		    },
    { IXON	,	"Enable start/stop output control" 	},
    { IXOFF ,	"Enable start/stop input control" 	},
    { 0	    ,   NULL                                }
};

struct flaginfo local_flags[] = {
    { ISIG	,	"Enable signals"                    },
    { ICANON,	"Canonical input (erase and kill)"  },
    { ECHO	,	"echo"                              },
    { ECHOE	,	"Echo ERASE as BS-SPACE-BS"         },
    { ECHOK	,	"Echo KILL by starting new line"	},
    { 0    	,   NULL                                }
};

struct flaginfo output_flags[] = {
    { OPOST, "Enable implementation-defined output processing" },
    { OLCUC, "Map lowercase characters to uppercase on output" },
    { ONOCR, "Don't output CR at column 0" },
    { OCRNL, "Map CR to NL on output" },
    { VTDLY, "Vertical tab delay mask" },
    { 0, NULL }
};

struct flaginfo control_flags[] = { 
    { CREAD, "enable receiver" },
    { PARENB, "enable parity generation on output and parity checking for input" },
    { CLOCAL, "Ignore modem control lines" },
    { 0, NULL }
};

void show_flagset( int thevalue, struct flaginfo thebitnames[] );
void show_some_flags( struct termios *ttyp )
/*
 *	show the values of two of the flag sets_: c_iflag and c_lflag
 *	adding c_oflag and c_cflag is pretty routine - just add new
 *	tables above and a bit more code below.
 */
{
    show_flagset( ttyp->c_iflag, input_flags );
    show_flagset( ttyp->c_lflag, local_flags );
    show_flagset( ttyp->c_oflag, output_flags );
    show_flagset( ttyp->c_cflag, control_flags );
}

void show_flagset( int thevalue, struct flaginfo thebitnames[] )
/*
 * check each bit pattern and display descriptive title
 */
{
    for ( int i=0; thebitnames[i].fl_value != 0 ; i++ ) {
        if ( thevalue & thebitnames[i].fl_value )
            printf("ON  ");
        else
            printf("OFF ");
        printf( "  %s\n", thebitnames[i].fl_name);
    }
}


void set_flagset( int thevalue, struct flaginfo thebitnames[] );
void set_flag( int thevalue, struct termios *ttyp )
/*
 *	set the values of two of the flag sets_: c_iflag and c_lflag
 *	adding c_oflag and c_cflag is pretty routine - just add new
 *	tables above and a bit more code below.
 */
{
    set_flagset( ttyp->c_iflag, input_flags );
    set_flagset( ttyp->c_lflag, local_flags );
    set_flagset( ttyp->c_oflag, output_flags );
    set_flagset( ttyp->c_cflag, control_flags );
}

void set_flagset( int thevalue, struct flaginfo thebitnames[] )
/*
 * check each bit pattern and display descriptive title
 */
{
    for ( int i=0; thebitnames[i].fl_value != 0 ; i++ ) {
        if ( thevalue & thebitnames[i].fl_value )
            printf("ON  ");
        else
            printf("OFF ");
        printf( "  %s\n", thebitnames[i].fl_name);
    }
}
