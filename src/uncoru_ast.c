#include <stdlib.h>
#include "uncoru.h"
#include "uncoru_ast.h"
#include "uncoru_token.h"
#include "print.h"

struct uncoru_ast_t {
    UNCORU_AST_TYPE ast_t;
    /* Add uncoru ast union later. */
};

uncoru_ast_t * uncoru_ast_new(UNCORU_AST_TYPE ast_t)
{
    /* Check whether ast_t is valid later. */

    uncoru_ast_t *ast = \
        (uncoru_ast_t *) malloc(sizeof(uncoru_ast_t));
    if (!ast) {
        PUTERR("Failed to allocate memory for uncoru ast");
        PUTERR("Check available system memory");
        return ast;
    }

    ast->ast_t = ast_t;

    /* Create uncoru ast union object later.*/

    return ast;
}

void uncoru_ast_delete(void *self)
{
    if (!self)
        return;

    free(self);
}
