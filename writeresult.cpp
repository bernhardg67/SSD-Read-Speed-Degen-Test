#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

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

    out << "# device_id\t inode_number\t Age_in_days\t  Size_in_MB\t"
           "Time \t\tMB_per_Second"
        << std::endl;

    out << std::right;
    for (struct scorecard sc : result_list) {
        out << "  " << std::setfill('0') << std::setw(2) << std::hex;
        out << sc.dev_major << ":" << sc.dev_minor << "\t\t";
        out << std::setfill(' ') << std::dec;
        out << std::setw(10) << sc.inode_num << "\t";
        out << std::setw(10) << std::fixed << std::setprecision(1) << sc.age / (3600*24) << "\t";
        out << std::setw(10) << std::fixed << std::setprecision(1)<< (double) sc.size / (1 << 20) << "\t";
        out << std::setw(7) << std::scientific << std::setprecision(2) << sc.t_elapsed << "\t";
        out << std::setw(10) << std::fixed << std::setprecision(1)  << sc.mb_per_sec << std::endl;
    }

    if (of.is_open())
        of.close();
}
