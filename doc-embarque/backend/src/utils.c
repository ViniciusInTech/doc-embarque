#include "../include/utils.h"

#include "paths.h"
#include "../include/paths.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif


void ensure_data_directory() {
#ifdef _WIN32
    _mkdir(DATA_DIR);
#else
    mkdir(DATA_DIR, 0777);
#endif
}