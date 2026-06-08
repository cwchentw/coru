#ifndef UNCORU_ARGUMENT_H
#define UNCORU_ARGUMENT_H

#include "uncoru_command.h"
#include "language.h"

typedef struct uncoru_argument_t {
    int argc;
    char **argv;
    int index;
    language_t lang;
    UNCORU_COMMAND cmd;
    char *path;
} uncoru_argument_t;

uncoru_argument_t * uncoru_argument_parse(int argc, char *argv[]);
void uncoru_argument_delete(void *self);
UNCORU_COMMAND uncoru_argument_command(uncoru_argument_t *self);
char * uncoru_argument_path(uncoru_argument_t *self);

#define uncoru_argument_language(arg) ((arg)->lang)

#endif  /* UNCORU_ARGUMENT_H */
