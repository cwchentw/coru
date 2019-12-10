#include "uncoru.h"
#include "uncoru_argument.h"
#include "uncoru_command.h"
#include "uncoru_help.h"
#include "uncoru_metadata.h"
#include "utils.h"


BOOL uncoru_run(int argc, char **argv, char **out)
{
    uncoru_argument_t *arg = uncoru_argument_parse(argc, argv);
    if (!arg)
        goto ERROR;

    UNCORU_COMMAND cmd = uncoru_argument_command(arg);

    if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_VERSION)) {
        uncoru_help_version();
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_TOO_FEW)) {
        PUTERR("No input file");
        goto ERROR;
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_LOAD)) {
        /* Pass. */
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_TOO_MANY)) {
        PUTERR("%s only accepts single file", UNCORU_PROGRAM);
        goto ERROR;
    }
    else {
        PUTERR("Unknown option");
        goto ERROR;
    }

    uncoru_argument_delete(arg);

    return 0;

ERROR:
    if (arg)
        uncoru_argument_delete(arg);

    return 1;
}
