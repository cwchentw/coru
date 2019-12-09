#ifndef CORU_ARGUMENT_H
#define CORU_ARGUMENT_H

#include "boolean.h"
#include "coru_command.h"
#include "language.h"

typedef struct coru_argument_t coru_argument_t;

coru_argument_t * coru_argument_parse(int argc, char **argv);
void coru_argument_delete(void *self);
BOOL coru_argument_is_all(coru_argument_t *self);
language_t coru_argument_language(coru_argument_t *self);
CORU_COMMAND coru_argument_command(coru_argument_t *self);
char * coru_argument_path(coru_argument_t *self);

#endif  /* CORU_ARGUMENT_H */
