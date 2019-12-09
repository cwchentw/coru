#include <stdlib.h>
#include "coru_argument.h"
#include "coru_help.h"
#include "coru_metadata.h"
#include "coru_run.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    coru_argument_t *arg = NULL;
    char *out = NULL;

    arg = coru_argument_parse(argc, argv);
    if (!arg) {
    #if DEBUG
        PUTERR("Failed to parse argument(s)");
    #endif
        goto ERROR;
    }

    if (!coru_run(arg, &out)) {
        goto ERROR;
    }

    if (out)
        PRINT("%s", out);

    free(out);
    coru_argument_delete((void *) arg);

    return 0;

ERROR:
    if (out)
        free(out);

    if (arg)
        coru_argument_delete((void *) arg);

    return 1;
}
