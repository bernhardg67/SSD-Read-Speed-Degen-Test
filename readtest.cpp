#include <fstream>
#include <iomanip>
#include <iostream>

#include "degentest.h"

void perform_readtest(std::vector<struct scorecard> &filelist,
                      std::string outfnam, size_t chunk_size, bool verbose) {

    // write results to file or stdout
    std::ofstream of;
    std::streambuf *buf;
    if (outfnam == "") {
        // no output file given, write to stdout
        buf = std::cout.rdbuf();
    } else {
        // write to file
        if (verbose)
            std::cout << "Writing output to " << outfnam << std::endl;
        of.open(outfnam);
        if (!of.is_open()) {
            std::cerr << "Can't write results to file " << outfnam << std::endl;
            exit(EXIT_FAILURE);
        }
        buf = of.rdbuf();
    }
    std::ostream out(buf);

    out << "# Age[day]\t  Size[MB]\tTime[s]\tSpeed[MB/s]\t\tFilename"
        << std::endl;

    // loop through all files, read them and output results
    for (unsigned long i = 0; i < filelist.size(); ++i) {
        struct scorecard &sc = filelist[i];

        // read file and measure time
        if (timedFileRead(sc.filename->c_str(), chunk_size, sc.t_elapsed) !=
            (ssize_t)sc.size) {
            // read error: set read speed to zero and skip output
            sc.mb_per_sec = 0.0;
            std::cerr << " failed reading " << *sc.filename << std::endl;
            continue;
        }
        sc.mb_per_sec =
            sc.t_elapsed ? sc.size / ((1 << 20) * sc.t_elapsed) : 0.0;

        // Write results to output
        out << std::right;

        // out << "  " << std::setfill('0') << std::setw(2) << std::hex;
        // out << sc.dev_major << ":" << sc.dev_minor << "\t\t";
        // out << std::setfill(' ') << std::dec;
        // out << std::setw(10) << sc.inode << "\t";
        out << std::setw(10) << std::fixed << std::setprecision(1)
            << sc.age / (3600 * 24) << "\t";
        out << std::setw(10) << std::fixed << std::setprecision(1)
            << (double)sc.size / (1 << 20) << "\t";
        out << std::setw(12) << std::scientific << std::setprecision(2)
            << sc.t_elapsed << "\t";
        out << std::setw(10) << std::fixed << std::right << std::setprecision(1)
            << sc.mb_per_sec << "\t\t";
        out << *sc.filename << std::endl;
    }

    if (of.is_open())
        of.close();
}
