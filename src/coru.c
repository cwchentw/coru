#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax_data.h"
#include "coru.h"
#include "coru_eval.h"
#include "coru_stats.h"
#include "hash_table.h"
#include "language.h"
#include "print.h"

static BOOL
_coru_load(
    FILE *stream,
    coru_stats_t *stats,
    language_t lang,
    BOOL is_all,
    char **out);

BOOL
coru_load_all_s(
    FILE *stream,
    coru_stats_t *stats,
    language_t lang,
    char **out)
{
    return _coru_load(stream, stats, lang, TRUE, out);
}

BOOL
coru_load_non_empty(
    FILE *stream,
    coru_stats_t *stats,
    language_t lang,
    char **out)
{
    return _coru_load(stream, stats, lang, FALSE, out);
}

extern hash_table_t *comment_single_start;
extern hash_table_t *comment_single_end;
extern hash_table_t *comment_multiple_start;
extern hash_table_t *comment_multiple_end;

static BOOL
_coru_load(
    FILE *stream,
    coru_stats_t *stats,
    language_t lang,
    BOOL is_all,
    char **out)
{
    char *line = NULL;
    char *more_line = NULL;
    coru_eval_t *eval = NULL;

    size_t line_size = 150;  /* Sensible line width. */
    line = (char *) malloc(line_size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate line object");
        goto ERROR_CORU_LOAD;
    }

    line[0] = '\0';

    eval = coru_eval_new();
    if (!eval)
        goto ERROR_CORU_LOAD;

    while (fgets(line, line_size, stream)) {
        if (line_size == strlen(line)) {
            if ('\n' != line[line_size - 1]) {
                line_size <<= 1;
                more_line = realloc(line, line_size);
                if (!more_line) {
                    PUTERR("Failed to realloc line buffer object");
                    PUTERR("Check available system memory");
                    goto ERROR_CORU_LOAD;
                }
                else {
                    line = more_line;
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

    coru_eval_delete(eval);
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
    if (eval)
        coru_eval_delete(eval);

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
