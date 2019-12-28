#include <stdio.h>
#include "print.h"
#include "uncoru.h"
#include "uncoru_argument.h"
#include "uncoru_command.h"
#include "uncoru_help.h"
#include "uncoru_metadata.h"

static BOOL uncoru_run_load(uncoru_argument_t *arg, char **out);

BOOL uncoru_run(int argc, char **argv, char **out)
{
    uncoru_argument_t *arg = uncoru_argument_parse(argc, argv);
    if (!arg)
        goto ERROR_UNCORU;

    UNCORU_COMMAND cmd = uncoru_argument_command(arg);

    if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_VERSION)) {
        uncoru_help_version();
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_LICENSE)) {
        uncoru_help_license();
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_HELP)) {
        uncoru_help_help(stdout);
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_TOO_FEW)) {
        PUTERR("No input file");
        goto ERROR_UNCORU;
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_LOAD)) {
        if (!uncoru_run_load(arg, out)) {
            PUTERR("Failed to load target file");
            goto ERROR_UNCORU;
        }
    }
    else if (is_uncoru_command_equal(cmd, UNCORU_COMMAND_TOO_MANY)) {
        PUTERR("%s only accepts single file", UNCORU_PROGRAM);
        goto ERROR_UNCORU;
    }
    else {
        PUTERR("Unknown option");
        uncoru_help_help(stderr);
        goto ERROR_UNCORU;
    }

    uncoru_argument_delete(arg);

    return TRUE;

ERROR_UNCORU:
    if (arg)
        uncoru_argument_delete(arg);

    return FALSE;
}

static BOOL uncoru_run_load(uncoru_argument_t *arg, char **out)
{
    FILE *fp = NULL;

    fp = fopen(uncoru_argument_path(arg), "r");
    if (!fp) {
        goto ERROR_LOAD;
    }

    fclose(fp);
    fp = NULL;

    return TRUE;

ERROR_LOAD:
    if (fp)
        fclose(fp);

    return FALSE;
}
