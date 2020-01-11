#include <assert.h>
#include <stdlib.h>
#include "coru_token.h"
#include "cstring.h"
#include "print.h"

struct coru_token_t {
    CORU_TOKEN_TYPE token_t;
    char *text;
};

coru_token_t * coru_token_new(CORU_TOKEN_TYPE t, char *text)
{
    coru_token_t *tn = (coru_token_t *) malloc(sizeof(coru_token_t));
    if (!tn) {
        PUTERR("Failed to allocate memory for token object");
        PUTERR("Check available system memory");
        return tn;
    }

    tn->token_t = t;
    tn->text = text;

    return tn;
}

CORU_TOKEN_TYPE coru_token_type(coru_token_t *self)
{
    assert(self);

    return self->token_t;
}

char * coru_token_text(coru_token_t *self)
{
    assert(self);

    return self->text;
}

coru_token_t * coru_token_copy(coru_token_t *self)
{
    assert(self);

    char *s = string_allocate(self->text);
    if (!s)
        return NULL;

    coru_token_t *token = coru_token_new(self->token_t, s);
    if (!token) {
        free(s);
        return NULL;
    }

    return token;
}

void coru_token_delete(void *self)
{
    if (!self)
        return;

    free(((coru_token_t *) self)->text);
    free(self);
}
