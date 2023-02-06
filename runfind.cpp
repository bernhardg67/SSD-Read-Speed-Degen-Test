#include <algorithm>
#include <assert.h>
#include <iostream>
#include <linux/limits.h>
#include <memory>

#include "degentest.h"

// read null terminated string from stream
// reads up to count - 1 characters
// null terminates string
// return value: last character read or EOF
int fget0(char *buf, int count, FILE *fp) {
    int c = EOF;
    while (--count > 0 && (c = fgetc(fp)) != EOF && c)
        *buf++ = c;
    *buf = '\0';
    return c;
}

// read list from find command
// return value: -1 execution of find failed, 0 success
int exec_find(const char *cmd, std::vector<struct scorecard> &filelist,
              size_t min_size, unsigned long &n_added,
              unsigned long &n_skipped) {
    char buffer[PATH_MAX + 1];
    std::string fnam;
    struct scorecard sc;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    // terminate if opening pipe to command fails
    if (!pipe)
        return -1;

    // read nullbyte terminated output from find command
    int c;
    n_added = n_skipped = 0;
    do {
        c = fget0(buffer, PATH_MAX + 1, pipe.get());
        if (buffer[0]) {
            fnam = buffer;
            while (c && c != EOF) {
                c = fget0(buffer, PATH_MAX + 1, pipe.get());
                if (buffer[0])
                    fnam += buffer;
            }
            // get file status information:
            // - ignore too small files (min_size)
            // - ignore files with negative age (mtime in future)
            if (call_statx(fnam.c_str(), sc.dev_major, sc.dev_minor, sc.inode,
                           sc.size, sc.age) ||
                sc.size < min_size || sc.age < 0) {
                ++n_skipped;
                if (sc.age < 0)
                    std::cerr << *sc.filename
                              << " has negative age. Sipping file." << std::endl;
                continue;
            }
            sc.filename = new std::string(fnam);
            filelist.push_back(sc);
            ++n_added;
        }
    } while (c != EOF);

    return 0;
}

// generate list of all files to be read
void generate_filelist(std::vector<std::string> &find_dirtrees, size_t min_size,
                       std::vector<struct scorecard> &filelist,
                       std::string find_options, bool verbose) {
    unsigned long n_added = 0;   // number of files added to list
    unsigned long n_skipped = 0; // number of files skipped

    // loop through all dirtrees
    for (std::string dirtree : find_dirtrees) {
        std::string findcmd =
            "find " + dirtree + " " + find_options + " -print0 2>/dev/null";
        if (verbose)
            std::cout << "Scanning directory tree " << dirtree
                      << ": find command: "
                      << "'" << findcmd << "'" << std::endl;

        // get list of files
        unsigned long na, ns;
        if (exec_find(findcmd.c_str(), filelist, min_size, na, ns)) {
            std::cerr << "execution of find command faild. Aborting."
                      << std::endl;
            exit(EXIT_FAILURE);
        }
        n_added += na;
        n_skipped += ns;
    }

    // sort list by age joung -> old
    std::sort(
        filelist.begin(), filelist.end(),
        [](struct scorecard &a, struct scorecard &b) { return a.age < b.age; });

    assert(n_added == filelist.size());

    if (verbose) {
        std::cout << "Scanning completed." << std::endl;
        std::cout << "Files added:   " << n_added << std::endl;
        std::cout << "Files skipped: " << n_skipped << std::endl;
    }
}
