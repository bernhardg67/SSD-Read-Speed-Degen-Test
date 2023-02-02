#include <iostream>
#include <string>
#include <vector>

#include "defaults.h"
#include "parsopt.h"
#include "readtest.h"
#include "runfind.h"
#include "timedread.h"
#include "writeresult.h"

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
    std::vector<std::string *> filelist;
    std::vector<struct scorecard> result_list;

    parse_commandline(argc, argv);

    if (verbose_flag)
        printsettings();

    generate_filelist(filelist);

    perform_readtest(filelist, result_list);

    write_results(result_list);

    return EXIT_SUCCESS;
}