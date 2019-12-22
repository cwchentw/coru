#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax_data.h"

#if _WIN32
    #include <windows.h>
    #include <shlwapi.h>
#elif __unix__ || __APPLE__
    #include <unistd.h>
    #include <sys/stat.h>
#else
    #error "Unsupported platform"
#endif

#include "coru.h"
#include "coru_argument.h"
#include "coru_command.h"
#include "coru_help.h"
#include "coru_metadata.h"
#include "coru_eval.h"
#include "coru_stats.h"
#include "hash_table.h"
#include "language.h"
#include "print.h"

static BOOL coru_run_load(coru_argument_t * arg, char **out);

BOOL coru_run(int argc, char **argv, char **out)
{
    coru_argument_t *arg = coru_argument_parse(argc, argv);
    if (!arg)
        goto ERROR_CORU;

    CORU_COMMAND cmd = coru_argument_command(arg);

    if (is_coru_command_equal(cmd, CORU_COMMAND_VERSION)) {
        coru_help_version();
    }
    else if (is_coru_command_equal(cmd, CORU_COMMAND_LICENSE)) {
        coru_help_license();
    }
    else if (is_coru_command_equal(cmd, CORU_COMMAND_HELP)) {
        coru_help_help(stdout);
    }
    else if (is_coru_command_equal(cmd, CORU_COMMAND_TOO_FEW)) {
        PUTERR("No input file");
        goto ERROR_CORU;
    }
    else if (is_coru_command_equal(cmd, CORU_COMMAND_LOAD)) {
        if (!coru_run_load(arg, out)) {
            PUTERR("Failed to load target file");
            goto ERROR_CORU;
        }
    }
    else if (is_coru_command_equal(cmd, CORU_COMMAND_TOO_MANY)) {
        PUTERR("%s only accepts single file", CORU_PROGRAM);
        goto ERROR_CORU;
    }
    else {
        PUTERR("Unknown option");
        coru_help_help(stderr);
        goto ERROR_CORU;
    }

    coru_argument_delete(arg);

    return TRUE;

ERROR_CORU:
    if (arg)
        coru_argument_delete(arg);

    return FALSE;
}

static BOOL coru_run_load(coru_argument_t * arg, char **out)
{
    coru_stats_t *stats = NULL;
    FILE *fp = NULL;

#if _WIN32
    if (!PathFileExists(coru_argument_path(arg))) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }
#elif __unix__ || __APPLE__
    struct stat st;

    if (stat(coru_argument_path(arg), &st) & F_OK) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }
#else
    #error "Unsupported platform"
#endif

    language_t lang = LANGUAGE_UNKNOWN;

    if (LANGUAGE_UNKNOWN != coru_argument_language(arg)) {
        lang =  coru_argument_language(arg);
    }
    else {
        /* detect_target_language() may detect target language by
           opening target source. Hence, we put the statement before
           fopen() statement. */
        lang = detect_target_language(coru_argument_path(arg));
    }

#if DEBUG
    if (is_language_equal(lang, LANGUAGE_UNKNOWN)) {
        PUTERR("Unsupported language");
    } else {
        PUTS("Target language: %s", language_to_string(lang));
    }
#endif

    fp = fopen(coru_argument_path(arg), "r");
    if (!fp) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }

    stats = coru_stats_load(fp);
    if (!stats)
        goto ERROR_LOAD;

    fclose(fp);

    fp = NULL;

#if DEBUG
    PUTS("Source width: %lu", coru_stats_width(stats));
    PUTS("Source height: %lu", coru_stats_height(stats));
    /* Add stats for non-comment lines. */
#endif

    fp = fopen(coru_argument_path(arg), "r");
    if (!fp) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }

    BOOL is_all = coru_argument_is_all(arg);

    if (is_all) {
        if (!coru_load_all(fp, stats, lang, out)) {
            goto ERROR_LOAD;
        }
    }
    else {
        if (!coru_load_non_empty(fp, stats, lang, out)) {
            goto ERROR_LOAD;
        }
    }

    /* Free system resources. */
    fclose(fp);
    coru_stats_delete((void *) stats);

    return TRUE;

ERROR_LOAD:
    if (fp)
        fclose(fp);

    if (stats)
        coru_stats_delete((void *) stats);

    return FALSE;
}

static BOOL _coru_load(FILE *stream, coru_stats_t *stats, language_t lang, BOOL is_all, char **out);

BOOL coru_load_all(FILE *stream, coru_stats_t *stats, language_t lang, char **out)
{
    return _coru_load(stream, stats, lang, TRUE, out);
}

BOOL coru_load_non_empty(FILE *stream, coru_stats_t *stats, language_t lang, char **out)
{
    return _coru_load(stream, stats, lang, FALSE, out);
}

extern hash_table_t *comment_single_start;
extern hash_table_t *comment_single_end;
extern hash_table_t *comment_multiple_start;
extern hash_table_t *comment_multiple_end;

static BOOL _coru_load(FILE *stream, coru_stats_t *stats, language_t lang, BOOL is_all, char **out)
{
    char *line = NULL;

    size_t line_size = 150;  /* Sensible line width. */
    line = (char *) malloc(line_size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate line object");
        goto ERROR_CORU_LOAD;
    }

    line[0] = '\0';

    coru_eval_t *eval = coru_eval_new();
    if (!eval)
        goto ERROR_CORU_LOAD;

    while (fgets(line, line_size, stream)) {
        size_t sz_space;
        size_t sz_start;
        size_t sz_end;
        if (line_size == strlen(line)) {
            if ('\n' != line[line_size - 1]) {
                line_size <<= 1;
                if (!realloc(line, line_size)) {
                    PUTERR("Failed to realloc line buffer object");
                    PUTERR("Check available system memory");
                    goto ERROR_CORU_LOAD;
                }
            }
            else {
                goto RELOAD_LINE;
            }
        }
        else {
RELOAD_LINE:
            if (!coru_eval_eval(eval, stats, lang, is_all, line, out))
                goto ERROR_CORU_LOAD;
        }
    }

    free(line);

    if (comment_multiple_end)
        hash_table_delete(comment_multiple_end);

    if (comment_multiple_start)
        hash_table_delete(comment_multiple_start);

    if (comment_single_end)
        hash_table_delete(comment_single_end);

    if (comment_single_start)
        hash_table_delete(comment_single_start);

    return TRUE;

ERROR_CORU_LOAD:
    if (line)
        free(line);

    if (comment_multiple_end)
        hash_table_delete(comment_multiple_end);

    if (comment_multiple_start)
        hash_table_delete(comment_multiple_start);

    if (comment_single_end)
        hash_table_delete(comment_single_end);

    if (comment_single_start)
        hash_table_delete(comment_single_start);

    return FALSE;
}
