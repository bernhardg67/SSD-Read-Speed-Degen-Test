#include "defaults.h"


bool verbose_flag = false;  // Flag set by ‘--verbose’. 

// compile time defaults
std::string find_options = "-xdev -type f";     //  options for find command
std::vector<std::string> find_dirtrees = {"."}; // start find there

size_t min_size = 1 << 19;   // smaller files are discarded
size_t chunk_size = 1 << 20; // read files in chunks of this size

std::string result_fnam; // write output to this file
