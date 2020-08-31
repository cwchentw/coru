#include <stdlib.h>
#include "print.h"
#include "uncoru_parser.h"

struct uncoru_parser_t {
    /* Dummy field. */  
};

uncoru_parser_t * uncoru_parser_new()
{
    uncoru_parser_t *parser = \
        (uncoru_parser_t *) malloc(sizeof(uncoru_parser_t));
    if (!parser) {
        PUTERR("Failed to allocate memory for Uncoru Parser");
        PUTERR("Check available system memory");
        return parser;
    }

    return parser;
}

void uncoru_parser_delete(void *self)
{
    if (!self)
        return;

    free(self);
}
