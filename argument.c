#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "argument.h"
#include "boolean.h"

struct lai_argument_t {
    int argc;
    char **argv;
    int index;
    COMMAND cmd;
};

lai_argument_t * lai_argument_parse(int argc, char **argv)
{
    lai_argument_t *arg = (lai_argument_t *) malloc(sizeof(lai_argument_t));
    if (!arg)
        return arg;

    arg->argc = argc;
    arg->argv = argv;
    arg->index = 1;

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
                arg->index += 1;
            }
        }
    }

    return arg;
}

void lai_argument_delete(void *self)
{
    assert(self);

    free(self);
}

COMMAND lai_argument_command(lai_argument_t *self)
{
    assert(self);

    return self->cmd;
}
