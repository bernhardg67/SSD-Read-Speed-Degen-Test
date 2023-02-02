#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "defaults.h"
#include "readtest.h"
#include "writeresult.h"


void write_results(std::vector<struct scorecard> &result_list) {
    // write results to file or stdout
    std::ofstream of;
    std::streambuf *buf;
    if (result_fnam == "") {
        // no output file given, write to stdout
        buf = std::cout.rdbuf();
    } else {
        // write to file
        if (verbose_flag)
            std::cout << "Writing output to " << result_fnam << std::endl;
        of.open(result_fnam);
        if (!of.is_open()) {
            std::cerr << "Can't write results to file " << result_fnam
                      << std::endl;
            exit(EXIT_FAILURE);
        }
        buf = of.rdbuf();
    }
    std::ostream out(buf);

    out << "# device_id\t inode_number\t Age_in_Seconds\t  Size_in_Bytes\t "
           "Time \t\tMB_per_Second"
        << std::endl;

    out << std::scientific;
    out << std::setprecision(4);

    for (struct scorecard sc : result_list) {
        out << std::setw(10) << sc.device_id << "\t";
        out << std::setw(10) << sc.inode_num << "\t";
        out << std::setw(10) << sc.age << "\t";
        out << std::setw(10) << sc.size << "\t";
        out << sc.t_elapsed << "\t";
        out << sc.mb_per_sec << std::endl;
    }

    if (of.is_open())
        of.close();
}
