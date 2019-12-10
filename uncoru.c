#include "uncoru.h"
#include "uncoru_argument.h"


BOOL uncoru_run(int argc, char **argv, char **out)
{
    uncoru_argument_t *arg = uncoru_argument_parse(argc, argv);
    if (arg)
        goto ERROR;

    uncoru_argument_delete(arg);

    return TRUE;

ERROR:
    if (arg)
        uncoru_argument_delete(arg);

    return FALSE;
}
