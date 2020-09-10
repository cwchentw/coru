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

static uncoru_ast_code_t * _uncoru_ast_code_new(void);
static void _uncoru_ast_code_delete(void *self);
static uncoru_ast_space_t * _uncoru_ast_space_new(void);
static void _uncoru_ast_space_delete(void *self);
static uncoru_ast_tab_t * _uncoru_ast_tab_new(void);
static void _uncoru_ast_tab_delete(void *self);

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

    if (UNCORU_AST_CODE == ast->ast_t) {
        ast->ast.code_t = _uncoru_ast_code_new();
        if (!(ast->ast.code_t)) {
            free(ast);
            return NULL;
        }
    }

    if (UNCORU_AST_CODE == ast->ast_t) {
        ast->ast.code_t = _uncoru_ast_code_new();
        if (!(ast->ast.code_t)) {
            free(ast);
            return NULL;
        }
    }
    else if (UNCORU_AST_SPACE == ast->ast_t) {
        ast->ast.space_t = _uncoru_ast_space_new();
        if (!(ast->ast.space_t)) {
            free(ast);
            return NULL;
        }
    }
    else if (UNCORU_AST_TAB == ast->ast_t) {
        ast->ast.tab_t = _uncoru_ast_tab_new();
        if (!(ast->ast.tab_t)) {
            free(ast);
            return NULL;
        }
    }

    return ast;
}

void uncoru_ast_delete(void *self)
{
    if (!self)
        return;

    UNCORU_AST_TYPE ast_t = ((uncoru_ast_t *) self)->ast_t;

    if (UNCORU_AST_CODE == ast_t) {
        uncoru_ast_code_t *ast = \
            ((uncoru_ast_t *) self)->ast.code_t;
        _uncoru_ast_code_delete(ast);
    }
    else if (UNCORU_AST_SPACE == ast_t) {
        uncoru_ast_space_t *ast = \
            ((uncoru_ast_t *) self)->ast.space_t;
        _uncoru_ast_space_delete(ast);
    }
    else if (UNCORU_AST_TAB == ast_t) {
        uncoru_ast_tab_t *ast = \
            ((uncoru_ast_t *) self)->ast.tab_t;
        _uncoru_ast_tab_delete(ast);
    }

    free(self);
}

struct uncoru_ast_code_t {
    size_t size;
    size_t capacity;
    uncoru_token_t **tokens;
};

static uncoru_ast_code_t * _uncoru_ast_code_new(void)
{
    uncoru_ast_code_t *ast = \
        (uncoru_ast_code_t *) malloc(sizeof(uncoru_ast_code_t));
    if (!ast) {
        PERROR("Failed to allocate memory for uncoru ast");
        PERROR("Check available system memory");
        return ast;
    }

    ast->size = 0;
    ast->capacity = 8;

    ast->tokens = \
        (uncoru_token_t **) \
        malloc(ast->capacity * sizeof(uncoru_token_t *));
    if (!(ast->tokens)) {
        PERROR("Failed to allocate memory for the tokens in uncoru ast");
        PERROR("Check available system memory");
        free(ast);
        return NULL;
    }

    {
        size_t i;
        for (i = 0; i < ast->capacity; i++)
            ast->tokens[i] = NULL;
    }

    return ast;    
}

static void _uncoru_ast_code_delete(void *self)
{
    if (!self)
        return;

    size_t size = ((uncoru_ast_code_t *) self)->capacity;
    uncoru_token_t **tokens = ((uncoru_ast_code_t *) self)->tokens;

    {
        size_t i;
        for (i = 0; i < size; i++) {
            if (tokens[i])
                uncoru_token_delete(tokens[i]);
        }
    }

    free(tokens);
    free(self);
}

struct uncoru_ast_space_t {
    size_t size;
    size_t capacity;
    uncoru_token_t *token;
};

static uncoru_ast_space_t * _uncoru_ast_space_new(void)
{
    uncoru_ast_space_t *ast = \
        (uncoru_ast_space_t *) malloc(sizeof(uncoru_ast_space_t));
    if (!ast) {
        PERROR("Failed to allocate memory for coru ast");
        PERROR("Check available system memory");
        return ast;
    }

    ast->size = 0;
    ast->capacity = 1;
    ast->token = NULL;

    return ast;
}

static void _uncoru_ast_space_delete(void *self)
{
    if (!self)
        return;

    uncoru_token_t *token = ((uncoru_ast_space_t *) self)->token;

    uncoru_token_delete(token);
    free(self);
}

struct uncoru_ast_tab_t {
    size_t size;
    size_t capacity;
    uncoru_token_t *token;
};

static uncoru_ast_tab_t * _uncoru_ast_tab_new(void)
{
    uncoru_ast_tab_t *ast = \
        (uncoru_ast_tab_t *) malloc(sizeof(uncoru_ast_tab_t));
    if (!ast) {
        PERROR("Failed to allocate memory for coru ast");
        PERROR("Check available system memory");
        return ast;
    }

    ast->size = 0;
    ast->capacity = 1;
    ast->token = NULL;

    return ast;
}

static void _uncoru_ast_tab_delete(void *self)
{
    if (!self)
        return;

    uncoru_token_t *token = ((uncoru_ast_tab_t *) self)->token;

    uncoru_token_delete(token);
    free(self);
}
