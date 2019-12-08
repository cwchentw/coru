#ifndef CORU_ARGUMENT_H
#define CORU_ARGUMENT_H

#include "boolean.h"
#include "command.h"

typedef struct coru_argument_t coru_argument_t;

coru_argument_t * coru_argument_parse(int argc, char **argv);
void coru_argument_delete(void *self);
COMMAND coru_argument_command(coru_argument_t *self);
char * coru_argument_path(coru_argument_t *self);

#endif  /* CORU_ARGUMENT_H */
