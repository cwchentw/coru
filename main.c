#include "argument.h"
#include "help.h"
#include "utils.h"

BOOL run(lai_argument_t *arg);

int main(int argc, char *argv[])
{
    lai_argument_t *arg = NULL;

    if (argc < 2) {
        PUTERR("No input file");
        goto ERROR;
    }

    arg = lai_argument_parse(argc, argv);
    if (!arg) {
        PUTERR("Failed to parse argument(s)");
        goto ERROR;
    }
    
    if (!run(arg)) {
        PUTERR("Unknown option");
        goto ERROR;
    }

    lai_argument_delete((void *) arg);

    return 0;

ERROR:
    if (arg)
        lai_argument_delete((void *) arg);

    return 1;
}

BOOL run(lai_argument_t *arg)
{
    if (is_command_equal(lai_argument_command(arg), COMMAND_VERSION)) {
        help_version();
        return TRUE;
    }
    else if (is_command_equal(lai_argument_command(arg), COMMAND_LICENSE)) {
        help_license();
        return TRUE;
    }

    return FALSE;
}
