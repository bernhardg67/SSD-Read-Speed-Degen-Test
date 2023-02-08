#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include "defaults.h"
#include "degentest.h"

// Print stetting
void printsettings() {
    std::cout << "verbose: " << verbose_flag << std::endl;
    std::cout << "find command options: " << find_options << std::endl;
    std::cout << "find dir trees: ";
    for (std::string s : find_dirtrees)
        std::cout << "'" << s << "' ";
    std::cout << std::endl;
    std::cout << "minimum file size: " << min_size << " Byte" << std::endl;
    std::cout << "file for results: " << result_fnam << std::endl;
}

int main(int argc, char **argv) {
    std::vector<struct scorecard> filelist;

    parse_commandline(argc, argv);

    // check if we have root privileges. If so we will drop the cache,
    // and set highest priority during read phase.
    // If not, we will still proceed and issue a warning.
    bool rootpriv = geteuid() == 0;
    if (!rootpriv)
        std::cout << "Warning! No root privilegs. Can't drop cache and "
                     "increase priority!"
                  << std::endl
                  << "Procceding with non-empty cache and normal priority. "
                     "This may distort the results."
                  << std::endl;

    if (verbose_flag)
        printsettings();

    generate_filelist(find_dirtrees, min_size, filelist, find_options,
                      verbose_flag);

    perform_readtest(filelist, result_fnam, chunk_size, rootpriv, verbose_flag);

    return EXIT_SUCCESS;
}
