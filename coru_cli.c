#include <stdlib.h>
#include "coru.h"
#include "coru_argument.h"
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
