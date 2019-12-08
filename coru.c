#include <stdlib.h>
#include "argument.h"
#include "help.h"
#include "metadata.h"
#include "run.h"
#include "utils.h"

BOOL run(coru_argument_t *arg);

int main(int argc, char *argv[])
{
    coru_argument_t *arg = NULL;

    arg = coru_argument_parse(argc, argv);
    if (!arg) {
    #if DEBUG
        PUTERR("Failed to parse argument(s)");
    #endif
        goto ERROR;
    }

    if (!coru_run(arg)) {
        goto ERROR;
    }

    coru_argument_delete((void *) arg);

    return 0;

ERROR:
    if (arg)
        coru_argument_delete((void *) arg);

    return 1;
}
