#ifndef CORU_AST_H
#define CORU_AST_H

#include "coru.h"
#include "coru_token.h"

typedef struct coru_ast_t coru_ast_t;

typedef unsigned char CORU_AST_TYPE;

#define CORU_AST_CODE       0
#define CORU_AST_TAB        1
#define CORU_AST_BACKSLASH  2
#define CORU_AST_AMPERSAND  3  /* Code wrapping for Fortran. */
#define CORU_AST_BACKTICK   4  /* Code wrapping for PowerShell. */
#define CORU_AST_STRING     5

coru_ast_t * coru_ast_new(CORU_AST_TYPE ast_t);
void coru_ast_delete(void *self);
BOOL coru_ast_add(coru_ast_t *self, coru_token_t *token);
CORU_AST_TYPE coru_ast_type(coru_ast_t *self);

#endif  /* CORU_AST_H */
