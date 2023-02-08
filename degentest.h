#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <string>
#include <sys/types.h>
#include <vector>

// store results in this structure
struct scorecard {
    std::string *filename; // filename
    dev_t dev_major;       // ID major of device
    dev_t dev_minor;       // ID minor of device containing file
    ino_t inode;           // inode number
    size_t size;           // file size in byts
    double age;            // age in seconds (last modified or created)
    double t_elapsed;      // time to read file
    double mb_per_sec;     // read speed rate in MB/s
};

int call_statx(const char *fnam, dev_t &dev_major, dev_t &dev_minor,
               ino_t &inode, size_t &size, double &age);

void generate_filelist(std::vector<std::string> &find_dirtrees, size_t min_size,
                       std::vector<struct scorecard> &filelist,
                       std::string find_options, bool verbose);

void parse_commandline(int argc, char **argv);

void perform_readtest(std::vector<struct scorecard> &filelist,
                      std::string outfnam, size_t chunk_size, bool rootpriv,
                      bool verbose);

ssize_t timedFileRead(const char *fnam, size_t chunksize, double &t_elapsed);

void usage(std::string prgname);