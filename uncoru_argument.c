#include <assert.h>
#include <stdlib.h>
#include "uncoru_argument.h"
#include "uncoru_command.h"
#include "utils.h"


struct uncoru_argument_t {
    int argc;
    char **argv;
    int index;
    UNCORU_COMMAND cmd;
    char *path;
};

uncoru_argument_t * uncoru_argument_parse(int argc, char *argv[])
{
    uncoru_argument_t *arg = (uncoru_argument_t *) malloc(sizeof(uncoru_argument_t));
    if (!arg) {
        PUTERR("Failed to allocate memory for uncoru argument object");
        PUTERR("Check available system memory");
        return arg;
    }

    arg->argc = argc;
    arg->argv = argv;
    arg->index = 1;
    arg->cmd = UNCORU_COMMAND_UNKNOWN;
    arg->path = NULL;

    if (argc < 2) {
        arg->cmd = UNCORU_COMMAND_TOO_FEW;
        return arg;
    }

    while (arg->index < arg->argc) {
        char *opt = arg->argv[arg->index];

        if (is_uncoru_command_equal(arg->cmd, UNCORU_COMMAND_LOAD)) {
            arg->cmd = UNCORU_COMMAND_TOO_MANY;
            break;
        }
        else {
            arg->cmd = UNCORU_COMMAND_LOAD;
            arg->path = opt;
            arg->index += 1;
        }
    }

    return arg;
}

void uncoru_argument_delete(void *self)
{
    if (!self)
        return;

    free(self);
}

UNCORU_COMMAND uncoru_argument_command(uncoru_argument_t *self)
{
    assert(self);

    return self->cmd;
}
