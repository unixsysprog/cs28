#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "sttyl.h"

int main(int argc, char *argv[])
{

    struct termios ttyinfo;
    if (tcgetattr(0, &ttyinfo) != 0) {
        perror("cannot get params");
        exit(EXIT_FAILURE);
    }

    struct winsize window;
    if (ioctl(0, TIOCGWINSZ, &window) != 0) {
        perror("cannot get terminal size");
        exit(EXIT_FAILURE);
    }

    // just reading settings
    if (argc == 1) { 
        read_settings(&ttyinfo, &window);
        return EXIT_SUCCESS;
    }

    // iterate through all other settings making sure they're
    // valid and then setting them.
    write_settings(&ttyinfo, argc-1, &argv[1]);

    return EXIT_SUCCESS;
}