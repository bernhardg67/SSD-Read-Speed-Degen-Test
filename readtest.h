#pragma one

#include <string>
#include <vector>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// store results in this structure
struct scorecard {
    std::string *filename; // filename
    dev_t device_id;       // ID of device containing file
    ino_t inode_num;       // inode number
    ssize_t size;          // file size in byts
    int age;               // age in seconds (last modified)
    double t_elapsed;      // time to read file
    double mb_per_sec;     // read speed rate in MB/s
};

void perform_readtest(std::vector<std::string *> &filelist,
                      std::vector<struct scorecard> &result_list);