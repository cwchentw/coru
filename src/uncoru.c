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
    char *more_line = NULL;
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
                more_line = realloc(line, sz_line);
                if (!more_line) {
                    PUTERR("Failed to reallocate line object");
                    PUTERR("Check available system memory");
                    goto ERROR_UNCORU_LOAD;
                }
                else {
                    line = more_line;
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
