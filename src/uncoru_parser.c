#include <assert.h>
#include <stdlib.h>
#include "print.h"
#include "uncoru_ast.h"
#include "uncoru_lexer.h"
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

static BOOL _uncoru_parser_expand(uncoru_parser_t *self);

BOOL uncoru_parser_parse(uncoru_parser_t *self, uncoru_lexer_t *lexer)
{
    assert(self);
    assert(lexer);

    uncoru_token_t *token = uncoru_lexer_peek_n(lexer, 1);
    while (token) {
        if (!_uncoru_parser_expand(self))
            return FALSE;

        /* Parse the tokens later. */
        break;  /* Remove it later. */
    }

    return TRUE;
}

static BOOL _uncoru_parser_expand(uncoru_parser_t *self)
{
    if (self->size < self->capacity)
        return TRUE;

    self->capacity <<= 1;
    uncoru_ast_t **old_asts = self->asts;
    uncoru_ast_t **new_asts = \
        (uncoru_ast_t **) \
        malloc(self->capacity * sizeof(uncoru_ast_t *));
    if (!new_asts)
        return FALSE;

    {
        size_t i = 0;
        while (i < self->size) {
            new_asts[i] = old_asts[i];
            i++;
        }
    }

    {
        size_t i;
        for (i = self->size; i < self->capacity; i++)
            new_asts[i] = NULL;
    }

    self->asts = new_asts;
    free(old_asts);

    return TRUE;
}

void uncoru_parser_delete(void *self)
{
    if (!self)
        return;

    free(self);
}
