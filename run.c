#include "argument.h"
#include "boolean.h"
#include "command.h"
#include "help.h"
#include "metadata.h"
#include "stats.h"
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
        lai_stats_t *stats = NULL;
        FILE *fp = NULL;

        stats = lai_stats_new();
        if (!stats) {
            PUTERR("Failed to load stats");
            goto ERROR_LOAD;
        }

        fp = fopen(lai_argument_path(arg), "r");
        if (!fp) {
            PUTERR("Failed to open file at %s", lai_argument_path(arg));
            goto ERROR_LOAD;
        }

        PUTERR("Not implemented yet");

        fclose(fp);

        lai_stats_delete((void *) stats);

        return FALSE;

ERROR_LOAD:
        if (fp)
            fclose(fp);

        if (stats)
            lai_stats_delete((void *) stats);

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
