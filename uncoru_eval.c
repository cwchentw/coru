#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "language.h"
#include "print.h"
#include "syntax_data.h"
#include "uncoru_eval.h"
#include "uncoru_stats.h"

struct uncoru_eval_t {
    /* Declare it later. */
};

uncoru_eval_t * uncoru_eval_new(void)
{
    uncoru_eval_t *eval = \
        (uncoru_eval_t *) malloc(sizeof(uncoru_eval_t));
    if (!eval) {
        PUTERR("Failed to allocate memory for uncoru eval object");
        PUTERR("Check available system memory");
        return eval;
    }

    return eval;
}

void uncoru_eval_delete(void *self)
{
    assert(self);

    free(self);
}

extern hash_table_t *comment_single_start;
extern hash_table_t *comment_single_end;

BOOL uncoru_eval_eval(
    uncoru_eval_t *self,
    uncoru_stats_t *stats,
    language_t lang,
    char *line, char **out)
{
    assert(self);

    if (!comment_single_start) {
        comment_single_start = init_comment_single_start();
        if (!comment_single_start)
            goto ERROR_UNCORU_EVAL;
    }

    if (!comment_single_end) {
        comment_single_end = init_comment_single_end();
        if (!comment_single_end)
            goto ERROR_UNCORU_EVAL;
    }

    char *lang_string = language_to_string(lang);

    char *comment_start = \
        hash_table_get(comment_single_start, lang_string);

    char *comment_end = \
        hash_table_get(comment_single_end, lang_string);

    size_t temp = uncoru_stats_height(stats);
    size_t digit = 1;
    while (temp >= 10) {
        temp /= 10;
        digit += 1;
    }

    size_t line_size = \
        uncoru_stats_width(stats)
        - 2  /* indent */
        - strlen(comment_start)
        - 1  /* space */
        - digit
        + strlen(END_OF_LINE);

    if (0 != strcmp("", comment_end)) {
        line_size -= 1; /* space */
        line_size -= strlen(comment_end);
    }

    return TRUE;

ERROR_UNCORU_EVAL:
    return FALSE;
}
