#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>
#include <unistd.h>

#define NRECS 1000
#define UTSIZE (sizeof(struct utmp))

static struct utmp utmpbuf[NRECS];
static int num_recs;
static int cur_rec;
static int fd_utmp = -1;

static int utmp_reload();

int utmp_open( char *filename ) {
    fd_utmp = open( filename, O_RDONLY );
    cur_rec = num_recs = NRECS;
    return fd_utmp;
}

struct utmp *utmp_next() {
    struct utmp *recp;

    if ( fd_utmp == -1 ) return NULL;
    if ( cur_rec == 0 && utmp_reload() <= 0 ) return NULL;

    recp = &(utmpbuf[cur_rec]);
    cur_rec--;
    return recp;
}

static int utmp_reload() {
    int amt_read;

    amt_read = read(fd_utmp, utmpbuf, NRECS*UTSIZE);
    if (amt_read < 0) return -1;

    num_recs = amt_read / UTSIZE;
    cur_rec = num_recs;
    return num_recs;
} 

int utmp_close() {
    int rv = 0;
    if (fd_utmp != -1) {
        rv = close( fd_utmp );
        fd_utmp = -1;
    }
    return rv;
}