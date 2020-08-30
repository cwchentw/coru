#include <stdlib.h>
#include "uncoru.h"

char ** uncoru_out_new()
{
    char ** out = (char **) malloc(sizeof(char *));
    if (!out)
        return out;

    *out = NULL;

    return out;
}

void uncoru_out_delete(void *self)
{
    if (!self)
        return;

    char **out = (char **) self;

    free(*out);
    free(out);
}