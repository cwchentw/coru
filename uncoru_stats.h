#ifndef UNCORU_STATS_H
#define UNCORU_STATS_H

#include <stdio.h>

typedef struct uncoru_stats_t uncoru_stats_t;

uncoru_stats_t * uncoru_stats_load(FILE *stream);
void uncoru_stats_delete(void *self);

#endif  /* UNCORU_STATS_H */
