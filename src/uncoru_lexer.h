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
    const char *comment_start;
    const char *comment_end;
} uncoru_lexer_t;

int uncoru_lexer_new(uncoru_lexer_t *lexer);
void uncoru_lexer_delete(uncoru_lexer_t *self);
BOOL uncoru_lexer_lex(uncoru_lexer_t *self, char *input);
uncoru_token_t * uncoru_lexer_next(uncoru_lexer_t *self);
uncoru_token_t * uncoru_lexer_peek_n(uncoru_lexer_t *self, size_t n);

#define uncoru_lexer_comment_start(self) ((self)->comment_start)
#define uncoru_lexer_comment_end(self) ((self)->comment_end)
#define uncoru_lexer_set_comment_start(self, comment) ((self)->comment_start = (comment))
#define uncoru_lexer_set_comment_end(self, comment) ((self)->comment_end = (comment))

#endif  /* UNCORU_LEXER_H */
