#include <stdlib.h>
#include "coru_lexer.h"
#include "coru_token.h"
#include "print_function.h"

struct coru_lexer_t {
    size_t size;
    size_t capacity;
    size_t index;
    coru_token_t **tokens;
};

coru_lexer_t * coru_lexer_new(char *input)
{
    coru_lexer_t *lexer = (coru_lexer_t *) malloc(sizeof(coru_lexer_t));
    if (!lexer) {
        PUTERR("Failed to allocate memory for coru lexer object");
        PUTERR("Check available system memory");
        return lexer;
    }

    lexer->size = 0;
    lexer->capacity = 16;
    lexer->index = 0;

    lexer->tokens = \
        (coru_token_t **) malloc(lexer->capacity * sizeof(coru_token_t *));
    if (!(lexer->tokens)) {
        PUTERR("Failed to allocate memory for tokens in coru lexer");
        PUTERR("Check available system memory");
        free(lexer);
        lexer = NULL;
        return lexer;
    }

    return lexer;
}

void coru_lexer_delete(void *self)
{
    if (self)
        return;

    coru_token_t **tokens = ((coru_lexer_t *) self)->tokens;

    {
        size_t size = ((coru_lexer_t *) self)->size;
        size_t i;
        for (i = 0; i < size; i++) {
            coru_token_delete(tokens[i]);
        }
    }

    free(tokens);
    free(self);
}
