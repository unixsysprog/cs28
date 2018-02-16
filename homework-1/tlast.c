#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <utmp.h>
#include <unistd.h>
#include <time.h>
#include "utmplib.c"

#define TRUE 1;
#define FALSE 0;
typedef int bool;

void print_line( struct utmp* );
void showtime(long);

int main(int argc, char *argv[]) {
    // need to do command line handling
    extern char* optarg;
    extern int optind;
    char c;
    bool show_efficiency = FALSE;
    bool err = FALSE; 

    struct utmp *utbuf;
    int         utmpfd;
    char        *info = WTMP_FILE;

    while ((c = getopt(argc, argv, "ef:")) != -1) {
        switch (c) {
        case 'e':
            show_efficiency = TRUE;
            break;
        case 'f':
            info = optarg;
            break;
        case '?': 
            return 1;
        }
    }
    
    char *terminal_name = argv[optind];
    if ( terminal_name == NULL) {
        printf("Please supply a terminal name.\n");    
        return 2;
    };

    utmpfd = utmp_open(info);
            printf("process-type: %d", utbuf->ut_type);

    if (utmpfd == -1) {
        printf("Cannot Open file: %s\n", info);
        return 2;
    }

    while( (utbuf = utmp_next()) != NULL ) { 
        // if terminal name doesn't match then ignore
        if (strcmp(utbuf->ut_line, terminal_name) != 0) continue; 
        print_line(utbuf);
    } 
    
    printf("\n");
    return 0;
}

void print_line(struct utmp *utbuf) { 
    switch (utbuf->ut_type) {
        case USER_PROCESS: 
            printf("%-8s ", utbuf->ut_user);
            printf("%-12s ", utbuf->ut_line);
            printf("%-16s ", utbuf->ut_host);
            showtime(utbuf->ut_time);
            break;
        case DEAD_PROCESS:
            printf(" - ");
            showtime(utbuf->ut_time);
            printf("\n");
    }
}

void showtime(long timeval) {
    char buffer[80];
    time_t time = timeval;
    strftime(buffer, 80, "%a %b %e %H:%M", localtime(&time));
    printf("%s", buffer);
}