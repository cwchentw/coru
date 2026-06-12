#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


struct uncoru_doc_t {
    char *raw_text;
    uncoru_stats_t *stats;
    language_t lang;
};

static BOOL
uncoru_load_fs(
    FILE *stream,
    char **out,
    uncoru_stats_t *stats,
    language_t lang);

uncoru_doc_t * uncoru_doc_load_fs(FILE *stream, language_t lang)
{
    uncoru_doc_t *doc = (uncoru_doc_t *) malloc(sizeof(uncoru_doc_t));
    if (!doc) return NULL;

    doc->lang = lang;

    doc->stats = uncoru_stats_load_fs(stream);
    if (!(doc->stats))
        goto ERROR_UNCORU_DOC_LOAD_FS;

    rewind(stream);

    doc->raw_text = NULL;

    if (!uncoru_load_fs(stream, &doc->raw_text, doc->stats, lang))
        goto ERROR_UNCORU_DOC_LOAD_FS;

    return doc;

ERROR_UNCORU_DOC_LOAD_FS:
    if (doc->raw_text)
        free(doc->raw_text);

    if (doc->stats)
        uncoru_stats_delete(doc->stats);

    if (doc)
        free(doc);

    return NULL;
}

const char * uncoru_doc_string(uncoru_doc_t *doc)
{
    return doc ? doc->raw_text : NULL;
}

void uncoru_doc_delete(uncoru_doc_t *doc)
{
    if (doc) {
        if (doc->raw_text)
            free(doc->raw_text);

        if (doc->stats)
            uncoru_stats_delete(doc->stats);

        free(doc);
    }
}

static BOOL
uncoru_load_fs(
    FILE *stream,
    char **out,
    uncoru_stats_t *stats,
    language_t lang)
{
    char *line = NULL;
    char *more_line = NULL;
    uncoru_eval_t eval;

    size_t sz_line = 150;  /* Sensible line width. */
    line = (char *) malloc(sz_line * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate C string");
        PUTERR("Check available system memory");
        goto ERROR_UNCORU_LOAD;
    }

    while (fgets(line, (int) sz_line, stream)) {
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
            if (!uncoru_eval_eval(&eval, stats, lang, line, out))
                goto ERROR_UNCORU_LOAD;
        }
    }

    free(line);

    return TRUE;

ERROR_UNCORU_LOAD:
    if (line)
        free(line);

    return FALSE;
}
