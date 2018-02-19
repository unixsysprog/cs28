#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BLOCK_SIZE 512

#define REGULAR     '0'
#define HARDLINK    '1'
#define SYMLINK     '2'
#define CHAR        '3'
#define BLOCK       '4'
#define DIRECTORY   '5'
#define FIFO        '6'
#define CONTIGUOUS  '7'


struct tar_context {
    char *current_dir;
    int file;
};

struct tar_header {
    char name[100];     // name of the file
    char mode[8];       // file mode
    char uid[8];        // owner userid
    char gid[8];        // owner group id
    char size[12];      // size of file
    char mtime[12];     // modification time
    char checksum[8];   // checksum
    char type;          // file type
    char linkname[100]; // name of linked file
    char ustar[8];      // ustar
    char uname[32];     // owner user name
    char gname[32];     // owner group name
    char devmajor[8];   // device major number
    char devminor[8];   // device minor number
    char prefix[155];   // file name prefix
};


typedef struct tar_context tar_t;
typedef struct tar_header tar_header_t;

int tarc_open(tar_t *, char *);
int tarc_close(tar_t *);

int tar_it(tar_t *, char *);
