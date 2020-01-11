#ifndef UNCORU_STATS_H
#define UNCORU_STATS_H

#include <stdio.h>
#include "uncoru.h"

uncoru_stats_t * uncoru_stats_load(FILE *stream);
void uncoru_stats_delete(void *self);
size_t uncoru_stats_width(uncoru_stats_t *self);
size_t uncoru_stats_height(uncoru_stats_t *self);
void uncoru_stats_set_width(uncoru_stats_t *self, size_t width);
void uncoru_stats_set_height(uncoru_stats_t *self, size_t height);

#endif  /* UNCORU_STATS_H */
