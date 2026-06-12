#ifndef UNCORU_STATS_H
#define UNCORU_STATS_H

#include <stdio.h>
#include "uncoru.h"

typedef struct uncoru_stats_t {
    size_t width;
    size_t height;
} uncoru_stats_t;

uncoru_stats_t * uncoru_stats_load_fs(FILE *stream);

#define uncoru_stats_delete(self) (free((self)))
#define uncoru_stats_width(self) ((self)->width)
#define uncoru_stats_height(self) ((self)->height)
#define uncoru_stats_set_width(self, w) ((self)->width = (w))
#define uncoru_stats_set_height(self, h) ((self)->height = (h))

#endif  /* UNCORU_STATS_H */
