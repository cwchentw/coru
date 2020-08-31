#include <stdlib.h>
#include "uncoru.h"
#include "uncoru_ast.h"
#include "uncoru_token.h"
#include "print.h"

typedef struct uncoru_ast_code_t uncoru_ast_code_t;
typedef struct uncoru_ast_space_t uncoru_ast_space_t;
typedef struct uncoru_ast_tab_t uncoru_ast_tab_t;
typedef struct uncoru_ast_backslash_t uncoru_ast_backslash_t;
typedef struct uncoru_ast_ampersand_t uncoru_ast_ampersand_t;
typedef struct uncoru_ast_line_number_t uncoru_ast_line_number_t;
typedef struct uncoru_ast_string_t uncoru_ast_string_t;

struct uncoru_ast_t {
    UNCORU_AST_TYPE ast_t;
    union {
        uncoru_ast_code_t *code_t;
        uncoru_ast_space_t *space_t;
        uncoru_ast_tab_t *tab_t;
        uncoru_ast_backslash_t *backslash_t;
        uncoru_ast_ampersand_t *ampersand_t;
        uncoru_ast_line_number_t *line_number_t;
        uncoru_ast_string_t *string_t;
    } ast;
};

#define _is_valid_ast_type(ast_t) \
    (UNCORU_AST_CODE == (ast_t) \
        || UNCORU_AST_SPACE == (ast_t) \
        || UNCORU_AST_TAB == (ast_t) \
        || UNCORU_AST_BACKSLASH == (ast_t) \
        || UNCORU_AST_LINE_NUNBER == (ast_t) \
        || UNCORU_AST_STRING == (ast_t))

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

void uncoru_ast_delete(void *self)
{
    if (!self)
        return;

    free(self);
}
