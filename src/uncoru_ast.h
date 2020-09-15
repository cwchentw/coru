#ifndef UNCORU_AST_H
#define UNCORU_AST_H

#include "uncoru.h"
#include "uncoru_token.h"

typedef struct uncoru_ast_t uncoru_ast_t;

typedef unsigned char UNCORU_AST_TYPE;

#define UNCORU_AST_CODE         0
#define UNCORU_AST_SPACE        1
#define UNCORU_AST_TAB          2
#define UNCORU_AST_BACKSLASH    3
#define UNCORU_AST_AMPERSAND    4  /* For Fortran multiline string. */
#define UNCORU_AST_LINE_NUNBER  5
#define UNCORU_AST_STRING       6

uncoru_ast_t * uncoru_ast_new(UNCORU_AST_TYPE ast_t);
void uncoru_ast_delete(void *self);
BOOL uncoru_ast_add(uncoru_ast_t *self, uncoru_token_t *token);
UNCORU_AST_TYPE uncoru_ast_type(uncoru_ast_t *self);
char * uncoru_ast_text(uncoru_ast_t *self);

#endif  /* CORU_AST_H */
