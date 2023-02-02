#pragma once

#include <string>
#include <sys/types.h>
#include <vector>

extern bool verbose_flag;  // Flag set by ‘--verbose’. 

// compile time defaults
extern std::string find_options; //  options for find command
extern std::vector<std::string>  find_dirtrees; // start find there
extern size_t min_size;   // smaller files are discarded
extern size_t chunk_size; // read files in chunks of this size
extern std::string result_fnam; // write output to this file
