#ifndef UNCORU_LEXER_H
#define UNCORU_LEXER_H

#include "uncoru.h"

typedef struct uncoru_lexer_t uncoru_lexer_t;

uncoru_lexer_t * uncoru_lexer_new(void);
void uncoru_lexer_delete(void *self);
BOOL uncoru_lexer_lex(uncoru_lexer_t *self, char *input);

#endif  /* UNCORU_LEXER_H */
