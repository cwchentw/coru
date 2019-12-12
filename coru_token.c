#include <stdlib.h>
#include "coru_token.h"
#include "print_function.h"

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

void coru_token_delete(void *self)
{
    if (!self)
        return;

    free(((coru_token_t *) self)->text);
    free(self);
}
