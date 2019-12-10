#ifndef UNCORU_COMMAND_H
#define UNCORU_COMMAND_H

#include "uncoru.h"

typedef char UNCORU_COMMAND;

#define UNCORU_COMMAND_UNKNOWN   0
#define UNCORU_COMMAND_TOO_FEW   1
#define UNCORU_COMMAND_LOAD      2
#define UNCORU_COMMAND_TOO_MANY  3

BOOL is_uncoru_command_equal(UNCORU_COMMAND a, UNCORU_COMMAND b);

#endif  /* UNCORU_COMMAND_H */
