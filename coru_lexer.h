#ifndef CORU_LEXER_H
#define CORU_LEXER_H

#include "coru.h"
#include "coru_token.h"

typedef struct coru_lexer_t coru_lexer_t;

coru_lexer_t * coru_lexer_new(void);
BOOL coru_lexer_lex(coru_lexer_t *self, char *input);
coru_token_t * coru_lexer_next(coru_lexer_t *self);
void coru_lexer_delete(void *self);

#endif  /* CORU_LEXER_H */
