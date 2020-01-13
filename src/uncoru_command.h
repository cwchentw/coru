#ifndef UNCORU_COMMAND_H
#define UNCORU_COMMAND_H

#include "uncoru.h"

typedef char UNCORU_COMMAND;

#define UNCORU_COMMAND_UNKNOWN   0
#define UNCORU_COMMAND_VERSION   1
#define UNCORU_COMMAND_LICENSE   2
#define UNCORU_COMMAND_HELP      3
#define UNCORU_COMMAND_TOO_FEW   4
#define UNCORU_COMMAND_LOAD      5
#define UNCORU_COMMAND_TOO_MANY  6

#define is_uncoru_command_equal(a, b) ((a) == (b))

#endif  /* UNCORU_COMMAND_H */
