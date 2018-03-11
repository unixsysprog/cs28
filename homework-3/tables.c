#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "tables.h"

/**
 *  Read Tables and Functions
 **/ 
struct flaginfo control_chars[] = {
    { VINTR  ,  "intr"   },
    { VERASE ,  "erase"  },
    { VKILL  ,  "kill"   },
    { VSTART ,  "start"  },
    { VSTOP  ,  "stop"   },
    { VWERASE,  "werase" },
    { 0      , NULL     }
};

struct flaginfo control_flags[] = { 
    { PARENB, "parenb" },
    { HUPCL , "hupcl"  },
    { CREAD , "cread"  },
    { CLOCAL, "clocal" },
    { 0     , NULL     }
};

struct flaginfo input_flags[] = { 
    { BRKINT,	"brkint" },
    { INPCK ,   "inpck"  },
    { ICRNL	,	"icrnl"  },
    { IXANY	,	"ixany"  },
    { ONLCR	,	"onlcr"  },
    { IGNBRK,	"ignbrk" },
    { IGNPAR,	"igcpar" },
    { 0	    ,   NULL     }
};

struct flaginfo local_flags[] = {
    { IEXTEN , "iexten" },
    { ECHO   , "echo"   },
    { ECHOE	 , "echoe"  },
    { ECHOK	 , "echok"	},
    { ISIG   , "isig"   },
    { ICANON , "icanon" },
    { 0    	 , NULL     }
};

struct flaginfo output_flags[] = {
    { OPOST, "opost" },
    { OLCUC, "olcuc" },
    { ONOCR, "onocr" },
    { OCRNL, "ocrnl" },
    { VTDLY, "vtdly" },
    { 0    , NULL }
};

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


void show_control_chars( struct termios * ttyp, struct flaginfo thebitnames[] )
{
    int ascii_char;
    for (int i = 0; thebitnames[i].fl_name != 0; i++) {
        ascii_char = ttyp->c_cc[thebitnames[i].fl_value];
 
        if (ascii_char < 32) {
            printf("%s = ^%c;", thebitnames[i].fl_name, ( ascii_char - 1 ) + 'A'); 
        } else if (ascii_char < 127) { 
            printf("%s = %c;", thebitnames[i].fl_name, ascii_char);
        } else {
            printf("%s = ^?;", thebitnames[i].fl_name);
        }

        if ((i + 1) % 3 == 0) {       // make a new line every third item
            putchar('\n'); 
        } else { 
            putchar(' ');
        }
    }
}

void show_some_flags( struct termios *ttyp )
/*
 *	show the values of two of the flag sets_: c_iflag and c_lflag
 *	adding c_oflag and c_cflag is pretty routine - just add new
 *	tables above and a bit more code below.
 */
{
    show_control_chars( ttyp, control_chars );
    show_flagset( ttyp->c_cflag, control_flags );
    show_flagset( ttyp->c_iflag, input_flags );
    show_flagset( ttyp->c_lflag, local_flags );
    show_flagset( ttyp->c_oflag, output_flags );
}

void show_flagset( int thevalue, struct flaginfo thebitnames[] )
/*
 * check each bit pattern and display descriptive title
 */
{
    for ( int i=0; thebitnames[i].fl_value != 0 ; i++ ) {
        if ( !(thevalue & thebitnames[i].fl_value) ) { 
            printf("-");
        }
        printf( "%s ", thebitnames[i].fl_name);
    }
    putchar('\n');
}


/**
 *  Write Tables and Functions
 **/

flag_desc_t flag_list[] = {
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

flag_desc_t lookup(char *flag_name)
/*
 * return a flaglist if the provided flag_name is a match
 */
{
    int i;
    for( i = 0; flag_list[i].key != NULL; i++ ) {
        if (strcmp(flag_name, flag_list[i].key) == 0) {
            break;
        }
    }
    return flag_list[i];
}

void set_control(struct termios *settings, flag_desc_t *flag_obj, int argc, char **argv, int idx)
// set the ascii char on the c_cc flag in termios struct
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

void turn_bit_off(struct termios *settings, flag_desc_t *flag_obj, char *arg)
/* 
 * turn the bit off for the correct flag
 */
{ 
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

void turn_bit_on(struct termios *settings, flag_desc_t *flag_obj, char *arg) 
/* 
 * turn the bit on for the correct flag
 */
{ 
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