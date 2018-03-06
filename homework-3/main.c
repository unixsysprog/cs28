#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <termios.h>

// all termbits are in:
// /usr/include/asm-generic/termbits.h

void show_baud(speed_t);
void show_some_flags( struct termios *);

int main(int argc, char *argv[])
{
    struct termios ttyinfo;
    if (tcgetattr(0, &ttyinfo) != 0) {
        perror("cannot get params");
        exit(1);
    }

    struct winsize window;
    if (ioctl(0, TIOCGWINSZ, &window) != 0) {
        perror("cannot get terminal size");
        exit(1);
    }

    // just reading settings
    if (argc == 1) { 
        show_baud(cfgetospeed(&ttyinfo));
        printf("rows %d; ", window.ws_row);
        printf("columns %d; ", window.ws_col);
        putchar('\n');
        printf("erase: Ctrl-%c\n", ttyinfo.c_cc[VERASE]-1+'A'); 
        printf("kill: Ctrl-%c\n", ttyinfo.c_cc[VKILL]-1+'A');

        show_some_flags( &ttyinfo );                            /* show misc. flags	*/
    } 

    // iterate through all other settings making sure they're
    // valid and then setting them.
    for (int i = 1; i < argc; i++) {
        printf("arg %d: %s\n", i, argv[i]);
    } 

    return 0;
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
	int	i;
	
	for ( i=0; thebitnames[i].fl_value != 0 ; i++ ) {
		if ( thevalue & thebitnames[i].fl_value )
			printf("ON  ");
		else
			printf("OFF ");
		printf( "  %s\n", thebitnames[i].fl_name);
	}
}