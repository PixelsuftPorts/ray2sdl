#pragma once

#define SUPPORT_TRACELOG
#define MAX_TRACELOG_MSG_LENGTH 1024

#ifdef PATH_MAX
#define MAX_FILEPATH_LENGTH PATH_MAX
#else
#define MAX_FILEPATH_LENGTH 4096
#endif
