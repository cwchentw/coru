#include <assert.h>
#include <stdlib.h>
#include "print.h"
#include "uncoru_lexer.h"
#include "uncoru_token.h"

struct uncoru_lexer_t {
    /* Refactor it later. */
    size_t size;
    size_t capacity;
    size_t index;
    uncoru_token_t **tokens;
};

uncoru_lexer_t * uncoru_lexer_new(void)
{
    uncoru_lexer_t *lexer = \
        (uncoru_lexer_t *) malloc(sizeof(uncoru_lexer_t));
    if (!lexer) {
        PUTERR("Failed to allocate memory for uncoru lexer object");
        PUTERR("Check available system memory");
        return lexer;
    }

    lexer->size = 0;
    lexer->capacity = 16;
    lexer->index = 0;

    lexer->tokens = \
        (uncoru_token_t **) malloc(lexer->capacity * sizeof(uncoru_token_t *));
    if (!(lexer->tokens)) {
        PUTERR("Failed to allocate the tokens of uncoru lexer");
        PUTERR("Check available system memory");
        free(lexer);
        return NULL;
    }

    {
        size_t i;
        for (i = 0; i < lexer->capacity; i++)
            lexer->tokens[i] = NULL;
    }

    return lexer;
}

void uncoru_lexer_delete(void *self)
{
    assert(self);

    size_t capacity = ((uncoru_lexer_t *) self)->capacity;
    uncoru_token_t **tokens = ((uncoru_lexer_t *) self)->tokens;
    {
        size_t i;
        for (i = 0; i < capacity; i++) {
            if (tokens[i])
                uncoru_token_delete((void *) tokens[i]);
        }
    }

    free((void *) tokens);
    free(self);
}

BOOL uncoru_lexer_lex(uncoru_lexer_t *self, char *line)
{
    assert(self);

    return TRUE;
}
