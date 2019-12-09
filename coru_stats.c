#include <assert.h>
#include <stdlib.h>
#include "coru_stats.h"
#include "utils.h"

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
