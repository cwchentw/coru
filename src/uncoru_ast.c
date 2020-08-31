#include <stdlib.h>
#include "uncoru.h"
#include "uncoru_ast.h"
#include "uncoru_token.h"
#include "print.h"

struct uncoru_ast_t {
    UNCORU_AST_TYPE ast_t;
    /* Add uncoru ast union later. */
};

static BOOL _is_valid_ast_type(UNCORU_AST_TYPE ast_t);

uncoru_ast_t * uncoru_ast_new(UNCORU_AST_TYPE ast_t)
{
    if (!_is_valid_ast_type(ast_t)) {
#if DEBUG
        DEBUG_INFO("Invalid uncoru AST type");
#endif
        return NULL;
    }

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

static BOOL _is_valid_ast_type(UNCORU_AST_TYPE ast_t)
{
    return UNCORU_AST_CODE == ast_t
        || UNCORU_AST_SPACE == ast_t
        || UNCORU_AST_TAB == ast_t
        || UNCORU_AST_BACKSLASH == ast_t
        || UNCORU_AST_LINE_NUNBER == ast_t
        || UNCORU_AST_STRING == ast_t;
}

void uncoru_ast_delete(void *self)
{
    if (!self)
        return;

    free(self);
}
