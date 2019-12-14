#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "coru_lexer.h"
#include "coru_stats.h"
#include "coru_token.h"
#include "print_function.h"

struct coru_stats_t
{
    size_t width;
    size_t height;
};

coru_stats_t * coru_stats_new()
{
    coru_stats_t *stats = (coru_stats_t *) malloc(sizeof(coru_stats_t));
    if (!stats) {
        PUTERR("Failed to allocate memory for stats object");
        PUTERR("Check available system memory");
        return stats;
    }

    stats->width = 0;
    stats->height = 0;

    return stats;
}

coru_stats_t * coru_stats_load(FILE *stream)
{
    coru_lexer_t *lexer = NULL;
    size_t line_size = 150;  /* Sensible line width */
    char *line = (char *) malloc(line_size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate line object");
        PUTERR("Check available system memory");
        return NULL;
    }

    coru_stats_t *stats = coru_stats_new();
    if (!stats)
        goto ERROR;

    size_t sz_line;
    while (fgets(line, line_size, stream)) {
        if (line_size == strlen(line)) {
            if ('\n' != line[line_size - 1]) {
                line_size <<= 1;
                if (!realloc(line, line_size)) {
                    PUTERR("Failed to realloc line buffer object");
                    PUTERR("Check available system memory");
                    goto ERROR;
                }
            }
            else {
                goto LOAD_LINE;
            }
        }
        else {
LOAD_LINE:
            lexer = coru_lexer_new();
            if (!lexer)
                goto ERROR;

            if (!coru_lexer_lex(lexer, line)) {
                PUTERR("Failed to lex input");
                coru_lexer_delete(lexer);
                goto ERROR;
            }

            /* Fix TAB issue */
            sz_line = strlen(line);
            {
                size_t i;
                for (i = 0; i < strlen(line); i++) {
                    if ('\t' == line[i])
                        sz_line += 8;
                }
            }

            if (strlen(line) > coru_stats_width(stats)) {
                coru_stats_set_width(stats, sz_line);
            }

            coru_stats_set_height(stats, coru_stats_height(stats) + 1);

            coru_lexer_delete(lexer);
            lexer = NULL;
        }
    }

    free(line);

    return stats;

ERROR:
    if (line)
        free(line);

    if (stats)
        coru_stats_delete(stats);

    return NULL;
}

void coru_stats_delete(void *self)
{
    assert(self);

    free(self);
}

size_t coru_stats_width(coru_stats_t *self)
{
    assert(self);

    return self->width;
}

size_t coru_stats_height(coru_stats_t *self)
{
    assert(self);

    return self->height;
}

void coru_stats_set_width(coru_stats_t *self, size_t width)
{
    assert(self);

    self->width = width;
}

void coru_stats_set_height(coru_stats_t *self, size_t height)
{
    assert(self);

    self->height = height;
}
