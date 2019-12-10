#ifndef UNCORU_ARGUMENT_H
#define UNCORU_ARGUMENT_H

#include "uncoru_command.h"


typedef struct uncoru_argument_t uncoru_argument_t;

uncoru_argument_t * uncoru_argument_parse(int argc, char *argv[]);
void uncoru_argument_delete(void *self);
UNCORU_COMMAND uncoru_argument_command(uncoru_argument_t *self);

#endif  /* UNCORU_ARGUMENT_H */
