#ifndef CORU_COMMAND_H
#define CORU_COMMAND_H

#include "coru.h"

typedef char CORU_COMMAND;

#define CORU_COMMAND_UNKNOWN   0
#define CORU_COMMAND_VERSION   1
#define CORU_COMMAND_LICENSE   2
#define CORU_COMMAND_HELP      3
#define CORU_COMMAND_TOO_FEW   4
#define CORU_COMMAND_LOAD      5
#define CORU_COMMAND_TOO_MANY  6

BOOL is_coru_command_equal(CORU_COMMAND a, CORU_COMMAND b);

#endif  /* CORU_CORU_COMMAND_H */
