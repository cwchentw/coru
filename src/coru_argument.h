#ifndef CORU_ARGUMENT_H
#define CORU_ARGUMENT_H

#include "boolean.h"
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

#define coru_argument_is_all(arg) ((arg)->is_all)
#define coru_argument_language(arg) ((arg)->lang)
#define coru_argument_command(arg) ((arg)->cmd)
#define coru_argument_path(arg) ((arg)->path)

#endif  /* CORU_ARGUMENT_H */
