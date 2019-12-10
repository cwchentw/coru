#include <stdlib.h>
#include "uncoru_argument.h"
#include "utils.h"

struct uncoru_argument_t {
    /* Implement it later. */
};

uncoru_argument_t * uncoru_argument_parse(int argc, char *argv[])
{
    uncoru_argument_t *arg = (uncoru_argument_t *) malloc(sizeof(uncoru_argument_t));
    if (!arg) {
        PUTERR("Failed to allocate memory for uncoru argument object");
        PUTERR("Check available system memory");
        return arg;
    }

    return arg;
}

void uncoru_argument_delete(void *self)
{
    if (!self)
        return;

    free(self);
}
