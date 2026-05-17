#ifndef CORU_LEXER_H
#define CORU_LEXER_H

#include "boolean.h"
#include "coru.h"
#include "coru_token.h"

typedef struct coru_lexer_t {
    size_t size;
    size_t capacity;
    size_t index;
    coru_token_t **tokens;
} coru_lexer_t;

int coru_lexer_new(coru_lexer_t *lexer);
void coru_lexer_delete(coru_lexer_t *self);
BOOL coru_lexer_lex(coru_lexer_t *self, char *input);
coru_token_t * coru_lexer_next(coru_lexer_t *self);

#endif  /* CORU_LEXER_H */
