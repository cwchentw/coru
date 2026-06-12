#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "boolean.h"
#include "print.h"
#include "uncoru.h"
#include "uncoru_stats.h"

struct uncoru_stats_t {
    size_t width;
    size_t height;
};

static uncoru_stats_t * uncoru_stats_new(void);
static BOOL uncoru_stats_eval(uncoru_stats_t *self, char *line);

uncoru_stats_t * uncoru_stats_load_fs(FILE *stream)
{
    char *line = NULL;
    char *more_line = NULL;
    uncoru_stats_t *stats = NULL;

    size_t line_size = 150;  /* Sensible default line width. */
    line = (char *) malloc(line_size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate memory for C string");
        PUTERR("Check available system memory");
        goto ERROR_UNCORU_STATS;
    }

    stats = uncoru_stats_new();
    if (!stats)
        goto ERROR_UNCORU_STATS;

    while (fgets(line, (int) line_size, stream)) {
        if (line_size == strlen(line)) {
            if ('\n' != line[line_size-1]) {
                line_size <<= 1;
                more_line = realloc(line, line_size);
                if (!more_line) {
                    PUTERR("Failed to realloc line buffer object");
                    PUTERR("Check available system memory");
                    goto ERROR_UNCORU_STATS;
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
            if (!uncoru_stats_eval(stats, line))
                goto ERROR_UNCORU_STATS;
        }
    }

    free(line);

    return stats;

ERROR_UNCORU_STATS:
    if (stats)
        uncoru_stats_delete(stats);

    if (line)
        free(line);

    return NULL;
}

static uncoru_stats_t * uncoru_stats_new(void)
{
    uncoru_stats_t *stats = \
        (uncoru_stats_t *) malloc(sizeof(uncoru_stats_t));
    if (!stats) {
        PUTERR("Failed to allocate memory for uncoru stats object");
        PUTERR("Check available system memory");
        return stats;
    }

    stats->width = 0;
    stats->height = 0;

    return stats;
}

static BOOL uncoru_stats_eval(uncoru_stats_t *self, char *line)
{
    assert(self);

    size_t sz_line = strlen(line);
    if (sz_line > uncoru_stats_width(self))
        uncoru_stats_set_width(self, sz_line);

    uncoru_stats_set_height(self, uncoru_stats_height(self) + 1);

    return TRUE;
}

void uncoru_stats_delete(void *self)
{
    if (!self)
        return;

    free(self);
}

size_t uncoru_stats_width(uncoru_stats_t *self)
{
    assert(self);

    return self->width;
}

size_t uncoru_stats_height(uncoru_stats_t *self)
{
    assert(self);

    return self->height;
}

void uncoru_stats_set_width(uncoru_stats_t *self, size_t width)
{
    assert(self);

    self->width = width;
}

void uncoru_stats_set_height(uncoru_stats_t *self, size_t height)
{
    assert(self);

    self->height = height;
}
