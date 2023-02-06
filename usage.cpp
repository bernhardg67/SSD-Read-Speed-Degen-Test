#include <iostream>
#include "degentest.h"

// print usage information
void usage(std::string prgname)
{
  std::cout << prgname << " [options]" << std::endl <<
      "Options:" << std::endl <<
      "-b | --brief          report errors only" << std::endl <<
      "-f | --fopt           options to find command (default '-xdev -type f')" << std::endl << 
      "-h | --help           Print this help" << std::endl <<
      "-v | --verbose        verbose output" << std::endl <<
      "-V | --version        Print the version number" << std::endl <<
      "-m | --minsize <size> discard smaller files" << std::endl <<
      "                      use size suffices 'cbkMG' as defined by GNU find programm" << std::endl <<
      "-o | --output <fnam>  write output to file (default stdout)" << std::endl <<
      "-p | --path           Path to start at, can be called multipe times" << std::endl <<
      std::endl <<
      "usage example:" << std::endl <<
      prgname << "-v -p / -p /home  -m 1M -o res.dat -f \"-xdev -type f -maxdepth 1\"" << std::endl <<
      "scan directory tree / and /home, test only file  >1MB, write output to res.dat" << std::endl <<
      "call find with options \"-xdev -type f -maxdepth 1\"" << std::endl;
}
