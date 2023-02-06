#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "degentest.h"

// read file in chunks of buffer size using low level i/o
// fnam: path name of file to read
// chunksize: read file in chunks of this size
// t_elapsed: elapes time for read is returned here
// result: bytes read (>=0) or error (-1)

ssize_t timedFileRead(const char *fnam, size_t chunksize, double &t_elapsed) {
    ssize_t nread = 0;
    ssize_t nr;
    struct timespec t_start, t_end;
    
    t_elapsed = 0.0; // initialize consistently, in case read fails

    // open file for reading (low level i/o)
    int fd = open(fnam, O_RDONLY);
    if (fd == -1)
        return -1; // open failed

    // allocate buffer for reading file in chunks
    char *buf = new char[chunksize];
    if (buf == NULL) {
        close(fd);
        return -1;
    }

    // read whole file in chunks of bufsize and measure duration
    clock_gettime(CLOCK_MONOTONIC_RAW, &t_start);
    do {
        nread += (nr = read(fd, buf, chunksize));
    } while (nr == (ssize_t) chunksize);
    clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);

    // elapsed time in seconds
    t_elapsed = t_end.tv_sec - t_start.tv_sec +
                (t_end.tv_nsec - t_start.tv_nsec) * 1e-9;

    // free memory for buffer
    delete[] buf;

    // close or last read could have failed (-1)
    // success: total number of bytes read
    return (!close(fd) && nr != -1) ? nread : -1;
}