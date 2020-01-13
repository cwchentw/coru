#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "language.h"
#include "print.h"
#include "syntax_data.h"
#include "uncoru_eval.h"
#include "uncoru_lexer.h"
#include "uncoru_stats.h"

struct uncoru_eval_t {
    /* Refactor it later. */
    BOOL first_line;
    BOOL mstart;
    BOOL mend;
    size_t multi;
    size_t line_number;
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

    char *comment_start = NULL;
    char *comment_end = NULL;
    uncoru_lexer_t *lexer = NULL;

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

    comment_start = \
        hash_table_get(comment_single_start, lang_string);

    comment_end = \
        hash_table_get(comment_single_end, lang_string);

    if (!(*out)) {
        *out = (char *) malloc(
            (uncoru_stats_width(stats) * uncoru_stats_height(stats) + 1)
             * sizeof(char));
        if (!(*out))
            goto ERROR_UNCORU_EVAL;

        (*out)[0] = '\0';
    }

    lexer = uncoru_lexer_new();
    if (!lexer)
        goto ERROR_UNCORU_EVAL;

    uncoru_lexer_set_comment_start(lexer, comment_start);
    uncoru_lexer_set_comment_end(lexer, comment_end);

    if (!uncoru_lexer_lex(lexer, line))
        goto ERROR_UNCORU_EVAL;

    uncoru_lexer_delete(lexer);

    return TRUE;

ERROR_UNCORU_EVAL:
    if (lexer)
        uncoru_lexer_delete(lexer);

    return FALSE;
}
