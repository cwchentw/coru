#ifndef CORU_AST_H
#define CORU_AST_H

#include "coru.h"
#include "coru_token.h"

typedef struct coru_ast_t coru_ast_t;

typedef unsigned char CORU_AST_TYPE;

#define CORU_AST_CODE       0
#define CORU_AST_TAB        1
#define CORU_AST_BACKSLASH  2
#define CORU_AST_AMPERSAND  3
#define CORU_AST_STRING     4

coru_ast_t * coru_ast_new(CORU_AST_TYPE ast_t);
BOOL coru_ast_add(coru_ast_t *self, coru_token_t *token);
CORU_AST_TYPE coru_ast_type(coru_ast_t *self);
void coru_ast_delete(void *self);

#endif  /* CORU_AST_H */
