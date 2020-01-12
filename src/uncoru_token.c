#include <assert.h>
#include <stdlib.h>
#include "print.h"
#include "uncoru_token.h"

struct uncoru_token_t {
    UNCORU_TOKEN_TYPE token_t;
    char *text;
};

#define IS_VALID_TOKEN_TYPE(t) \
    (UNCORU_TOKEN_CODE == (t) \
     || UNCORU_TOKEN_SPACE == (t) \
     || UNCORU_TOKEN_TAB == (t) \
     || UNCORU_TOKEN_SINGLE_QUOTE == (t) \
     || UNCORU_TOKEN_DOUBLE_QUOTE == (t) \
     || UNCORU_TOKEN_BACKSLASH == (t))

uncoru_token_t * uncoru_token_new(UNCORU_TOKEN_TYPE type, char *text)
{
    assert(IS_VALID_TOKEN_TYPE(type));

    uncoru_token_t * token = \
        (uncoru_token_t *) malloc(sizeof(uncoru_token_t *));
    if (!token) {
        PUTERR("Failed to allocate memory for uncoru token");
        PUTERR("Check available system memory");
        return token;
    }

    token->token_t = type;
    token->text = NULL;

    return token;
}

void uncoru_token_delete(void *self)
{
    assert(self);

    char *text = ((uncoru_token_t *) self)->text;
    if (text)
        free((void *) text);

    free(self);
}
