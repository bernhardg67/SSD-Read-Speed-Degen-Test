#include <iostream>
#include <string>
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

    if (verbose_flag)
        printsettings();

    generate_filelist(find_dirtrees, min_size, filelist, find_options,
                      verbose_flag);

    perform_readtest(filelist, result_fnam, chunk_size, verbose_flag);

    return EXIT_SUCCESS;
}