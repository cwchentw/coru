#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "language.h"
#include "print.h"
#include "syntax_data.h"
#include "uncoru.h"
#include "uncoru_argument.h"
#include "uncoru_command.h"
#include "uncoru_eval.h"
#include "uncoru_help.h"
#include "uncoru_metadata.h"
#include "uncoru_stats.h"


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
    uncoru_stats_t *stats = NULL;

    language_t lang = \
        detect_target_language(uncoru_argument_path(arg));

    fp = fopen(uncoru_argument_path(arg), "r");
    if (!fp)
        goto ERROR_LOAD;

    stats = uncoru_stats_load(fp);
    if (!stats)
        goto ERROR_LOAD;
#if DEBUG
    PUTS("Source width: %lu", uncoru_stats_width(stats));
    PUTS("Source height: %lu", uncoru_stats_height(stats));
#endif

    fclose(fp);
    fp = NULL;

    fp = fopen(uncoru_argument_path(arg), "r");
    if (!fp)
        goto ERROR_LOAD;

    if (!uncoru_load(fp, stats, lang, out))
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

extern hash_table_t *comment_single_start;
extern hash_table_t *comment_single_end;

BOOL
uncoru_load(
    FILE *stream,
    uncoru_stats_t *stats,
    language_t lang,
    char **out)
{
    char *line = NULL;
    uncoru_eval_t *eval = NULL;

    eval = uncoru_eval_new();
    if (!eval)
        goto ERROR_UNCORU_LOAD;

    size_t sz_line = 150;  /* Sensible line width. */
    line = (char *) malloc(sz_line * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate C string");
        PUTERR("Check available system memory");
        goto ERROR_UNCORU_LOAD;
    }

    while (fgets(line, sz_line, stream)) {
        if (sz_line == strlen(line)) {
            if ('\n' != line[sz_line-1]) {
                sz_line <<= 1;
                if (!realloc(line, sz_line)) {
                    PUTERR("Failed to reallocate line object");
                    PUTERR("Check available system memory");
                    goto ERROR_UNCORU_LOAD;
                }
            }
            else {
                goto LOAD_LINE;
            }
        }
        else {
        LOAD_LINE:
            if (!uncoru_eval_eval(eval, stats, lang, line, out))
                goto ERROR_UNCORU_LOAD;
        }
    }

    free(line);
    hash_table_delete(comment_single_end);
    hash_table_delete(comment_single_start);
    uncoru_eval_delete(eval);

    return TRUE;

ERROR_UNCORU_LOAD:
    if (line)
        free(line);

    if (comment_single_end)
        hash_table_delete(comment_single_end);

    if (comment_single_start)
        hash_table_delete(comment_single_start);

    if (eval)
        uncoru_eval_delete(eval);

    return FALSE;
}
