#include <assert.h>
#include <stdlib.h>
#include "print.h"
#include "uncoru_lexer.h"

struct uncoru_lexer_t {
    /* Refactor it later. */
    size_t size;
    size_t capacity;
    size_t index;
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

    return lexer;
}

void uncoru_lexer_delete(void *self)
{
    assert(self);

    free(self);
}

BOOL uncoru_lexer_lex(uncoru_lexer_t *self, char *line)
{
    assert(self);

    return TRUE;
}
