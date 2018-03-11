#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "sttyl.h" 
#include "tables.h"

void read_settings(struct termios *ttyinfo, struct winsize *window)
{ 
    show_baud(cfgetospeed(ttyinfo));
    printf("rows %d; ", window->ws_row);
    printf("columns %d; ", window->ws_col);
    putchar('\n');
    show_some_flags( ttyinfo );                            /* show misc. flags	*/
    putchar(8);
} 


void write_settings(struct termios *settings, int argc, char **argv )
{
    flag_desc_t flag_obj;
 
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {                 // check for negates
            flag_obj = lookup(&argv[i][1]); 
            turn_bit_off(settings, &flag_obj, argv[i]);
            continue;
        }                                 

        flag_obj = lookup(argv[i]); 
        if ( flag_obj.flag == C_CC ) {       // handle C_CC flags separately
            set_control(settings, &flag_obj, argc, argv, ++i); 
        } else {
            turn_bit_on(settings, &flag_obj, argv[i]);
        }
        
    } 

    if ( tcsetattr(0, TCSANOW, settings ) != 0 ) {
        perror("sttyl");
        exit(EXIT_FAILURE);
    }
}