#ifndef CORU_STATS_H
#define CORU_STATS_H

#include <stddef.h>
#include <stdio.h>
#include "coru.h"

typedef struct coru_stats_t {
    size_t width;
    size_t display_width;
    size_t height;
} coru_stats_t;

coru_stats_t * coru_stats_load_fs(FILE *stream);

#define coru_stats_delete(s) (free(s))
#define coru_stats_width(s) ((s)->width)
#define coru_stats_display_width(s) ((s)->display_width)
#define coru_stats_height(s) ((s)->height)
#define coru_stats_set_width(s, w) ((s)->width = (w))
#define coru_stats_set_display_width(s, w) ((s)->display_width = (w))
#define coru_stats_set_height(s, h) ((s)->height = (h))

#endif  /* CORU_STATS_H */
