#pragma once

#include <cstddef>

ssize_t timedFileRead(const char *fnam, size_t chunksize, double &t_elapsed);