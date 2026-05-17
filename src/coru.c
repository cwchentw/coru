#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boolean.h"
#include "coru.h"
#include "coru_eval.h"
#include "coru_stats.h"
#include "hash_table.h"
#include "language.h"
#include "print.h"
#include "syntax_data.h"

struct coru_doc_t {
    char *raw_text;
    coru_stats_t *stats;
    language_t lang;
};

static BOOL
_coru_load(
    FILE *stream,
    coru_stats_t *stats,
    language_t lang,
    BOOL is_all,
    char **out);

static BOOL
coru_load_all_fs(
    FILE *stream,
    char **out,
    coru_stats_t *stats,
    language_t lang)
{
    return _coru_load(stream, stats, lang, TRUE, out);
}

static BOOL
coru_load_non_empty_fs(
    FILE *stream,
    char **out,
    coru_stats_t *stats,
    language_t lang)
{
    return _coru_load(stream, stats, lang, FALSE, out);
}

extern hash_table_t *comment_multiple_start;
extern hash_table_t *comment_multiple_end;

coru_doc_t *coru_doc_load_all_fs(FILE *stream, language_t lang)
{
    coru_doc_t *doc = malloc(sizeof(coru_doc_t));
    if (!doc) return NULL;

    doc->lang = lang;

    doc->stats = coru_stats_load_fs(stream);

    if (!(doc->stats))
        goto ERROR_CORU_DOC_LOAD_ALL_FS;

    rewind(stream);

    doc->raw_text = NULL;

    if(!coru_load_all_fs(stream, &doc->raw_text, doc->stats, lang))
        goto ERROR_CORU_DOC_LOAD_ALL_FS;

    return doc;

ERROR_CORU_DOC_LOAD_ALL_FS:
    if (doc->raw_text)
        free(doc->raw_text);

    if (doc->stats)
        coru_stats_delete(doc->stats);

    if (doc)
        free(doc);

    return NULL;
}

coru_doc_t *coru_doc_load_non_empty_fs(FILE *stream, language_t lang)
{
    coru_doc_t *doc = malloc(sizeof(coru_doc_t));
    if (!doc) return NULL;

    doc->lang = lang;

    doc->stats = coru_stats_load_fs(stream);
    if (!(doc->stats))
        goto ERROR_CORU_DOC_LOAD_NON_EMPTY_FS;

    rewind(stream);

    doc->raw_text = NULL;

    if (!coru_load_non_empty_fs(stream, &doc->raw_text, doc->stats, lang))
        goto ERROR_CORU_DOC_LOAD_NON_EMPTY_FS;

    return doc;

ERROR_CORU_DOC_LOAD_NON_EMPTY_FS:
    if (doc->raw_text)
        free(doc->raw_text);

    if (doc->stats)
        coru_stats_delete(doc->stats);

    if (doc)
        free(doc);

    return NULL;
}

const char * coru_doc_string(const coru_doc_t *self)
{
    return self ? self->raw_text : NULL;
}

void coru_doc_delete(coru_doc_t *self)
{
    if (self) {
        if (self->raw_text) {
            free(self->raw_text);
        }

        if (self->stats) {
            coru_stats_delete(self->stats);
        }

        free(self);
    }
}

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
    coru_eval_t eval;

    size_t line_size = 150;  /* Sensible line width. */
    line = (char *) malloc(line_size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate line object");
        goto ERROR_CORU_LOAD;
    }

    line[0] = '\0';

    if (coru_eval_new(&eval))
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
            /* Remove the trailing newline. */
            line[strcspn(line, "\n")] = 0;

            if (!coru_eval_eval(&eval, stats, lang, is_all, line, out))
                goto ERROR_CORU_LOAD;
        }
    }

    free(line);

    if (comment_multiple_end)
        hash_table_delete(comment_multiple_end);

    if (comment_multiple_start)
        hash_table_delete(comment_multiple_start);

    return TRUE;

ERROR_CORU_LOAD:
    if (line)
        free(line);

    if (comment_multiple_end)
        hash_table_delete(comment_multiple_end);

    if (comment_multiple_start)
        hash_table_delete(comment_multiple_start);

    return FALSE;
}
