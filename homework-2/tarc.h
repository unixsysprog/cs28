#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct tar_context
{
    char *current_dir;
    int file;
};

typedef struct tar_context tar_t;

int tarc_open(tar_t *, char *);
int tarc_close(tar_t *);

int tar_it(tar_t *, char *);
