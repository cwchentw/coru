#include <assert.h>
#include <stdlib.h>
#include "cstring.h"
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
     || UNCORU_TOKEN_INTEGER == (t) \
     || UNCORU_TOKEN_COMMENT_START == (t) \
     || UNCORU_TOKEN_COMMENT_END == (t) \
     || UNCORU_TOKEN_SINGLE_QUOTE == (t) \
     || UNCORU_TOKEN_DOUBLE_QUOTE == (t) \
     || UNCORU_TOKEN_BACKSLASH == (t))

uncoru_token_t * uncoru_token_new(UNCORU_TOKEN_TYPE type, char *text)
{
    assert(IS_VALID_TOKEN_TYPE(type));
    assert(text);

    uncoru_token_t * token = \
        (uncoru_token_t *) malloc(sizeof(uncoru_token_t));
    if (!token) {
        PUTERR("Failed to allocate memory for uncoru token");
        PUTERR("Check available system memory");
        return token;
    }

    token->token_t = type;
    token->text = text;

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

uncoru_token_t * uncoru_token_copy(uncoru_token_t *self)
{
    assert(self);

    char *s = string_allocate(self->text);
    if (!s)
        return NULL;

    uncoru_token_t *token = uncoru_token_new(self->token_t, s);
    if (!token) {
        free(s);
        return NULL;
    }

    return token;
}

UNCORU_TOKEN_TYPE uncoru_token_type(uncoru_token_t *self)
{
    assert(self);

    return self->token_t;
}

char * uncoru_token_text(uncoru_token_t *self)
{
    assert(self);

    return self->text;
}
