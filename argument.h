#ifndef LAI_ARGUMENT_H
#define LAI_ARGUMENT_H

#include "boolean.h"
#include "command.h"

typedef struct lai_argument_t lai_argument_t;

lai_argument_t * lai_argument_parse(int argc, char **argv);
void lai_argument_delete(void *self);
COMMAND lai_argument_command(lai_argument_t *self);
char * lai_argument_path(lai_argument_t *self);

#endif  /* LAI_ARGUMENT_H */
