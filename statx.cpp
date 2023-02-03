#include "statx.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <ctype.h>
#include <errno.h>
#include <linux/fcntl.h>
#include <linux/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// convert statx_timestamp to seconds elapsed
double ts2sec_el(struct statx_timestamp t_stamp) {
    struct timespec t_now;
    clock_gettime(CLOCK_REALTIME, &t_now);
    return t_now.tv_sec - t_stamp.tv_sec +
           (t_now.tv_nsec - t_stamp.tv_nsec) * 1e-9;
}

// get statx information about file
int call_statx(std::string *fnam, struct scorecard &sc) {
    struct statx stx;

    memset(&stx, 0xbf, sizeof(stx));
    if (statx(AT_FDCWD, fnam->c_str(), AT_SYMLINK_NOFOLLOW,
              STATX_BASIC_STATS | STATX_BTIME, &stx) < 0)
        return -1; // statx failed

    sc.filename = fnam;
    sc.size = (stx.stx_mask & STATX_SIZE) ? sc.size = stx.stx_size : 0;
    sc.inode_num = (stx.stx_mask & STATX_INO) ? stx.stx_ino : 0;
    sc.dev_major = stx.stx_dev_major;
    sc.dev_minor = stx.stx_dev_minor;

    // calculate file age (= last write operation)
    // mtime can differ from last write to file
    // typically for installed software (/usr etc.) mtime may be older than file
    // creation if btime (file created) is more recent than mtime (last
    // modified) use btime because a file can't be older than creation time this
    // does not solve the problem entirely because mtime can be set to any time,
    // e.g. even a time in the future. However in most cases this approach
    // should provide the true file age.
    double m_elapsed =
        (stx.stx_mask & STATX_MTIME) ? ts2sec_el(stx.stx_mtime) : -1.;
    double b_elapsed =
        (stx.stx_mask & STATX_BTIME) ? ts2sec_el(stx.stx_btime) : -1.;
    sc.age = (b_elapsed < m_elapsed) ? b_elapsed : m_elapsed;

    return 0;
}
