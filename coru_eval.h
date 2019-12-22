#ifndef CORU_EVAL_H
#define CORU_EVAL_H

#include "coru.h"

typedef struct coru_eval_t coru_eval_t;

coru_eval_t * coru_eval_new(void);
BOOL coru_eval_eval(
    coru_eval_t *self,
    coru_stats_t *stats,
    language_t lang,
    BOOL is_all,
    char *line, char **out);
void coru_eval_delete(void *self);

#endif  /* CORU_EVAL_H */
