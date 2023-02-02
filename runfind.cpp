#include <iostream>
#include <memory>
#include <iostream>
#include <linux/limits.h>

#include "defaults.h"
#include "runfind.h"

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
int exec_find(const char *cmd, std::vector<std::string *> &filelist) {
    char buffer[PATH_MAX + 1];
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    // terminate if opening pipe to command fails
    if (!pipe)
        return -1;

    // read nullbyte terminated output from find command
    int c;
    do {
        c = fget0(buffer, PATH_MAX + 1, pipe.get());
        if (buffer[0]) {
            std::string *result = new std::string;
            *result = buffer;
            while (c && c != EOF) {
                c = fget0(buffer, PATH_MAX + 1, pipe.get());
                if (buffer[0])
                    *result += buffer;
            }
            filelist.push_back(result);
        }
    } while (c != EOF);

    return 0;
}

// generate list of all files to be read
void generate_filelist(std::vector<std::string *> &filelist) {
    // loop through all dirtrees
    for (std::string dirtree : find_dirtrees) {
        std::string findcmd =
            "find " + dirtree + " " + find_options + " -print0 2>/dev/null";

        if (verbose_flag)
            std::cout << "find command: "
                      << "'" << findcmd << "'" << std::endl;

        // get list of files
        if (exec_find(findcmd.c_str(), filelist)) {
            std::cerr << "execution of find command faild. Aborting."
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    if (verbose_flag)
        std::cout << "Files found: " << filelist.size() << std::endl;
}
