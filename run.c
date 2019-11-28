#include "argument.h"
#include "boolean.h"
#include "command.h"
#include "help.h"
#include "metadata.h"
#include "utils.h"

BOOL lai_run(lai_argument_t *arg, char *out)
{
    if (is_command_equal(lai_argument_command(arg), COMMAND_VERSION)) {
        help_version();
        return TRUE;
    }
    else if (is_command_equal(lai_argument_command(arg), COMMAND_LICENSE)) {
        help_license();
        return TRUE;
    }
    else if (is_command_equal(lai_argument_command(arg), COMMAND_TOO_FEW)) {
        PUTERR("No input file");
        return FALSE;
    }
    else if (is_command_equal(lai_argument_command(arg), COMMAND_LOAD)) {
        PUTERR("Not implemented yet");
        return FALSE;
    }
    else if (is_command_equal(lai_argument_command(arg), COMMAND_TOO_MANY)) {
        PUTERR("%s only accepts single file", LAI_PROGRAM);
        return FALSE;
    }
    else {
        PUTERR("Unknown option");
        return FALSE;
    }
}
