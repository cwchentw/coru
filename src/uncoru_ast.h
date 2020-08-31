#ifndef UNCORU_AST_H
#define UNCORU_AST_H

#include "uncoru.h"
#include "uncoru_token.h"

typedef struct uncoru_ast_t uncoru_ast_t;

typedef unsigned char UNCORU_AST_TYPE;

uncoru_ast_t * uncoru_ast_new(UNCORU_AST_TYPE ast_t);
void uncoru_ast_delete(void *self);

#endif  /* CORU_AST_H */
