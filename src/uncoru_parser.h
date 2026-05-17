#ifndef UNCORU_PARSER_H
#define UNCORU_PARSER_H

#include "boolean.h"
#include "uncoru.h"
#include "uncoru_ast.h"
#include "uncoru_lexer.h"

typedef struct uncoru_parser_t {
    size_t size;
    size_t capacity;
    size_t index;
    uncoru_ast_t **asts;
} uncoru_parser_t;

int uncoru_parser_new(uncoru_parser_t *parser);
void uncoru_parser_delete(uncoru_parser_t *self);
BOOL uncoru_parser_parse(uncoru_parser_t *self, uncoru_lexer_t *lexer);
uncoru_ast_t * uncoru_parser_next(uncoru_parser_t *self);
uncoru_ast_t * uncoru_parser_peek_n(uncoru_parser_t *self, size_t n);

#endif  /* CORU_PARSER_H */
