#ifndef UNCORU_LEXER_H
#define UNCORU_LEXER_H

#include "boolean.h"
#include "uncoru.h"
#include "uncoru_token.h"

typedef struct uncoru_lexer_t {
    size_t size;
    size_t capacity;
    size_t index;
    uncoru_token_t **tokens;
    char *comment_start;
    char *comment_end;
} uncoru_lexer_t;

int uncoru_lexer_new(uncoru_lexer_t *lexer);
void uncoru_lexer_delete(uncoru_lexer_t *self);
void uncoru_lexer_set_comment_start(uncoru_lexer_t *self, char *comment);
void uncoru_lexer_set_comment_end(uncoru_lexer_t *self, char *comment);
char * uncoru_lexer_comment_start(uncoru_lexer_t *self);
char * uncoru_lexer_comment_end(uncoru_lexer_t *self);
BOOL uncoru_lexer_lex(uncoru_lexer_t *self, char *input);
uncoru_token_t * uncoru_lexer_next(uncoru_lexer_t *self);
uncoru_token_t * uncoru_lexer_peek_n(uncoru_lexer_t *self, size_t n);

#endif  /* UNCORU_LEXER_H */
