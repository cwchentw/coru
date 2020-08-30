#include <stdlib.h>
#include "language.h"
#include "print.h"
#include "uncoru.h"
#include "uncoru_argument.h"
#include "uncoru_command.h"
#include "uncoru_help.h"
#include "uncoru_metadata.h"
#include "uncoru_stats.h"

static BOOL uncoru_run(int argc, char **argv, char **out);

int main(int argc, char *argv[])
{
    char **out = (char **) malloc(sizeof(char *));
    if (!out) {
        PUTERR("Failed to allocate memory for output");
        PUTERR("Check available system memory");
        return 1;
    }

    *out = NULL;

    if (!uncoru_run(argc, argv, out))
        goto ERROR_UNCORU;

    if (*out)
        PRINT("%s", *out);

    free(*out);
    free(out);

    return 0;

ERROR_UNCORU:
    if (*out)
        free(*out);

    if (out)
        free(out);

    return 1;
}

static BOOL uncoru_run_load(uncoru_argument_t *arg, char **out);

static BOOL uncoru_run(int argc, char **argv, char **out)
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
    uncoru_stats_t *stats = NULL;

    language_t lang = \
        detect_target_language(uncoru_argument_path(arg));

#if _MSC_VER
    if (0 != fopen_s(&fp, uncoru_argument_path(arg), "r"))
        goto ERROR_LOAD;
#else
    fp = fopen(uncoru_argument_path(arg), "r");
    if (!fp)
        goto ERROR_LOAD;
#endif

    stats = uncoru_stats_load_fs(fp);
    if (!stats)
        goto ERROR_LOAD;
#if DEBUG
    PUTS("Source width: %lu", uncoru_stats_width(stats));
    PUTS("Source height: %lu", uncoru_stats_height(stats));
#endif

    rewind(fp);

    if (!uncoru_load_fs(fp, out, stats, lang))
        goto ERROR_LOAD;

    uncoru_stats_delete(stats);
    fclose(fp);

    return TRUE;

ERROR_LOAD:
    if (stats)
        uncoru_stats_delete(stats);

    if (fp)
        fclose(fp);

    return FALSE;
}
