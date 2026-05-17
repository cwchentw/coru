#ifndef CORU_EVAL_H
#define CORU_EVAL_H

#include "coru.h"
#include "coru_stats.h"

typedef struct coru_eval_t {
    BOOL first_line;
    BOOL mstart;
    BOOL mend;
    size_t multi;
    size_t line_number;
} coru_eval_t;

int coru_eval_new(coru_eval_t *eval);
BOOL coru_eval_eval(
    coru_eval_t *self,
    coru_stats_t *stats,
    language_t lang,
    BOOL is_all,
    char *line, char **out);

#endif  /* CORU_EVAL_H */
