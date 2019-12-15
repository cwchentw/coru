#ifndef CORU_PARSER_H
#define CORU_PARSER_H

#include "coru.h"
#include "coru_ast.h"
#include "coru_lexer.h"

typedef struct coru_parser_t coru_parser_t;

coru_parser_t * coru_parser_new(void);
BOOL coru_parser_parse(coru_parser_t *self, coru_lexer_t *lexer);
coru_ast_t * coru_parser_next(coru_parser_t *self);
void coru_parser_delete(void *self);

#endif  /* CORU_PARSER_H */
