#ifndef CORU_ARGUMENT_H
#define CORU_ARGUMENT_H

#include "coru.h"
#include "coru_command.h"
#include "language.h"

typedef struct coru_argument_t {
    int argc;
    char **argv;
    int index;
    BOOL is_all;
    language_t lang;
    CORU_COMMAND cmd;
    char *path;
} coru_argument_t;

int coru_argument_parse(coru_argument_t *arg, int argc, char **argv);
BOOL coru_argument_is_all(const coru_argument_t *self);
language_t coru_argument_language(const coru_argument_t *self);
CORU_COMMAND coru_argument_command(const coru_argument_t *self);
char * coru_argument_path(const coru_argument_t *self);

#endif  /* CORU_ARGUMENT_H */
