#ifndef CORU_PARSER_H
#define CORU_PARSER_H

#include "coru.h"
#include "coru_ast.h"
#include "coru_lexer.h"

typedef struct coru_parser_t {
    size_t size;
    size_t capacity;
    size_t index;
    coru_ast_t **asts;
} coru_parser_t;

int coru_parser_new(coru_parser_t *parser);
void coru_parser_delete(coru_parser_t *self);
BOOL coru_parser_parse(coru_parser_t *self, coru_lexer_t *lexer);
coru_ast_t * coru_parser_next(coru_parser_t *self);

#endif  /* CORU_PARSER_H */
