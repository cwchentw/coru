#ifndef CORU_RUN_H
#define CORU_RUN_H

#include "boolean.h"
#include "coru_argument.h"
#include "coru_stats.h"
#include "language.h"


BOOL coru_run(coru_argument_t *arg, char **out);
BOOL coru_load_all(FILE *stream, char **out, coru_stats_t *stats, language_t lang);
BOOL coru_load_non_empty(FILE *stream, char **out, coru_stats_t *stats, language_t lang);

#endif  /* CORU_RUN_H */
