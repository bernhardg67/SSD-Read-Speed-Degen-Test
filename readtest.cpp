#include <fstream>
#include <iomanip>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>



#include "degentest.h"

// If run as root: sync and drop cache, set maximum priority
void dropcache(bool verbose) {

    // set maximum priority (-20)
    if (setpriority(PRIO_PROCESS, getpid(), -20))
        std::cout << "Faild setting maximum priority. Proceeding with normal "
                     "priority."
                  << std::endl;

    if (verbose)
        std::cout << "Syncing and dropping cashes. May cause system freeze "
                     "for a few seconds."
                  << std::endl;
    int result =
        system("(sync && echo 3 > /proc/sys/vm/drop_caches) 2>/dev/null");
    if (!result && verbose)
        std::cout << "Done." << std::endl;
    else if (result)
        std::cout << "Warning! Dopping cache failed. Procseeding with "
                     "non-empty cache."
                  << std::endl;
}

void perform_readtest(std::vector<struct scorecard> &filelist,
                      std::string outfnam, size_t chunk_size, bool rootpriv,
                      bool verbose) {

    // drop cache and set maximum priority
    if (rootpriv)
        dropcache(verbose);

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
