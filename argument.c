#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "argument.h"
#include "boolean.h"
#include "utils.h"

struct coru_argument_t {
    int argc;
    char **argv;
    int index;
    COMMAND cmd;
    char *path;
};

coru_argument_t * coru_argument_parse(int argc, char **argv)
{
    coru_argument_t *arg = (coru_argument_t *) malloc(sizeof(coru_argument_t));
    if (!arg) {
        PUTERR("Failed to allocate memory for argument object");
        PUTERR("Check available system memory");
        return arg;
    }

    arg->argc = argc;
    arg->argv = argv;
    arg->index = 1;
    arg->cmd = COMMAND_UNKNOWN;
    arg->path = NULL;

    if (argc < 2) {
        arg->cmd = COMMAND_TOO_FEW;
        return arg;
    }

    while (arg->index < arg->argc) {
        if (0 == strcmp(arg->argv[arg->index], "-v")
            || 0 == strcmp(arg->argv[arg->index], "--version")) {
            arg->cmd = COMMAND_VERSION;
            break;
        }
        else if (0 == strcmp(arg->argv[arg->index], "--license")) {
            arg->cmd = COMMAND_LICENSE;
            break;
        }
        else if (0 == strcmp(arg->argv[arg->index], "-h")
            || 0 == strcmp(arg->argv[arg->index], "--help")) {
            arg->cmd = COMMAND_HELP;
            break;
        }
        else if ('-' == arg->argv[arg->index][0]) {
            arg->cmd = COMMAND_UNKNOWN;
            break;
        }
        else {
            if (COMMAND_LOAD == arg->cmd) {
                arg->cmd = COMMAND_TOO_MANY;
                break;
            }
            else {
                arg->cmd = COMMAND_LOAD;
                arg->path = arg->argv[arg->index];
                arg->index += 1;
            }
        }
    }

    return arg;
}

void coru_argument_delete(void *self)
{
    assert(self);

    free(self);
}

COMMAND coru_argument_command(coru_argument_t *self)
{
    assert(self);

    return self->cmd;
}

char * coru_argument_path(coru_argument_t *self)
{
    assert(self);

    return self->path;
}
