#ifndef LAI_STATS_H
#define LAI_STATS_H

#include <stddef.h>

typedef struct coru_stats_t coru_stats_t;

coru_stats_t * coru_stats_new();
void coru_stats_delete(void *self);
size_t coru_stats_width(coru_stats_t *self);
size_t coru_stats_height(coru_stats_t *self);
void coru_stats_set_width(coru_stats_t *self, size_t width);
void coru_stats_set_height(coru_stats_t *self, size_t height);

#endif  /* LAI_STATS_H */
