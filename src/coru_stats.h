#ifndef CORU_STATS_H
#define CORU_STATS_H

#include <stddef.h>
#include <stdio.h>
#include "coru.h"

typedef struct coru_stats_t {
    size_t width;
    size_t height;
} coru_stats_t;

coru_stats_t * coru_stats_load_fs(FILE *stream);
size_t coru_stats_width(coru_stats_t *self);
size_t coru_stats_height(coru_stats_t *self);
void coru_stats_set_width(coru_stats_t *self, size_t width);
void coru_stats_set_height(coru_stats_t *self, size_t height);
void coru_stats_delete(coru_stats_t *self);

#endif  /* CORU_STATS_H */
