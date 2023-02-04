#include <algorithm>
#include <iostream>

#include "defaults.h"
#include "readtest.h"
#include "timedread.h"
#include "statx.h"

// perform read speed test for all files > min_size

void perform_readtest(std::vector<std::string *> &filelist,
                      std::vector<struct scorecard> &result_list) {
    struct scorecard sc;

    for (std::string *fnam : filelist) {

        // get file status information, ignore too small files (min_size)
        if (call_statx(fnam, sc) || sc.size < min_size)
            continue; 

        // read file and measure time
        if (timedFileRead(fnam->c_str(), chunk_size, sc.t_elapsed) != (ssize_t)sc.size) {
            std::cerr << " failed reading " << *fnam << std::endl;
            continue;
        }
        sc.mb_per_sec =
            sc.t_elapsed ? sc.size / ((1 << 20) * sc.t_elapsed) : -1.0;
        result_list.push_back(sc); // store result in list
    }

    // sort list by age joung -> old
    std::sort(
        result_list.begin(), result_list.end(),
        [](struct scorecard &a, struct scorecard &b) { return a.age < b.age; });

    if (verbose_flag)
        std::cout << "Files read/reported: " << result_list.size() << std::endl;
}
