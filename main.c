#include "argument.h"
#include "help.h"
#include "metadata.h"
#include "run.h"
#include "utils.h"

BOOL run(lai_argument_t *arg);

int main(int argc, char *argv[])
{
    lai_argument_t *arg = NULL;

    arg = lai_argument_parse(argc, argv);
    if (!arg) {
        PUTERR("Failed to parse argument(s)");
        goto ERROR;
    }

    if (!lai_run(arg)) {
        goto ERROR;
    }

    lai_argument_delete((void *) arg);

    return 0;

ERROR:
    if (arg)
        lai_argument_delete((void *) arg);

    return 1;
}
