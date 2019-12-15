#include <stdlib.h>
#include "coru_parser.h"
#include "print.h"

struct coru_parser_t {
    /* Declare it later. */
};

coru_parser_t * coru_parser_new(void)
{
    coru_parser_t *parser = \
        (coru_parser_t *) malloc(sizeof(coru_parser_t));
    if (!parser) {
        PUTERR("Failed to allocate memory for Coru Parser");
        PUTERR("Check available system memory");
        return parser;
    }

    /* Init `parser` later */

    return parser;
}

void coru_parser_delete(void *self)
{
    if (!self)
        return;

    free(self);
}
