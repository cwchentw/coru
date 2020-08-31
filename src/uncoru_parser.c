#include <stdlib.h>
#include "print.h"
#include "uncoru_ast.h"
#include "uncoru_parser.h"

struct uncoru_parser_t {
    size_t size;
    size_t capacity;
    size_t index;
    uncoru_ast_t **asts;
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

    parser->size = 0;
    parser->capacity = 16;
    parser->index = 0;

    parser->asts = \
        (uncoru_ast_t **) \
        malloc(parser->capacity * sizeof(uncoru_ast_t *));
    if (!(parser->asts)) {
        PUTERR("Failed to allocate memory for internal ast array of Coru Parser");
        PUTERR("Check available system memory");
        free(parser);
        return NULL;
    }

    {
        size_t i;
        for (i = 0; i < parser->capacity; i++)
            parser->asts[i] = NULL;
    }

    return parser;
}

void uncoru_parser_delete(void *self)
{
    if (!self)
        return;

    free(self);
}
