#include <assert.h>
#include <stdlib.h>
#include "stats.h"

struct lai_stats_t
{
    size_t width;
    size_t height;
};

lai_stats_t * lai_stats_new()
{
    lai_stats_t *stats = (lai_stats_t *) malloc(sizeof(lai_stats_t));
    if (!stats)
        return stats;

    stats->width = 0;
    stats->height = 0;

    return stats;
}

void lai_stats_delete(void *self)
{
    assert(self);

    free(self);
}

size_t lai_stats_width(lai_stats_t *self)
{
    assert(self);

    return self->width;
}

size_t lai_stats_height(lai_stats_t *self)
{
    assert(self);

    return self->height;
}

void lai_stats_set_width(lai_stats_t *self, size_t width)
{
    assert(self);

    self->width = width;
}

void lai_stats_set_height(lai_stats_t *self, size_t height)
{
    assert(self);

    self->height = height;
}
