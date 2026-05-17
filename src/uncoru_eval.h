#ifndef UNCORU_EVAL_H
#define UNCORU_EVAL_H

#include "uncoru.h"
#include "uncoru_stats.h"

typedef struct uncoru_eval_t uncoru_eval_t;

uncoru_eval_t * uncoru_eval_new(void);
void uncoru_eval_delete(void *self);
BOOL uncoru_eval_eval(
    uncoru_eval_t *self,
    uncoru_stats_t *stats,
    language_t lang,
    char *line, char **out);

#endif  /* UNCORU_EVAL_H */
