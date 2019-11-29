#ifndef LAI_STATS_H
#define LAI_STATS_H

#include <stddef.h>

typedef struct lai_stats_t lai_stats_t;

lai_stats_t * lai_stats_new();
void lai_stats_delete(void *self);
size_t lai_stats_width(lai_stats_t *self);
size_t lai_stats_height(lai_stats_t *self);
void lai_stats_set_width(lai_stats_t *self, size_t width);
void lai_stats_set_height(lai_stats_t *self, size_t height);

#endif  /* LAI_STATS_H */
