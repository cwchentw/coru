#include <stdlib.h>
#include "coru.h"
#include "print.h"

int main(int argc, char *argv[])
{
    char *out = NULL;

    if (!coru_run(argc, argv, &out)) {
        goto ERROR;
    }

    if (out)
        PRINT("%s", out);

    if (out)
        free(out);

    return 0;

ERROR:
    if (out)
        free(out);

    return 1;
}
