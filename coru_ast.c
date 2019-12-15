#include <stdlib.h>
#include "coru.h"
#include "coru_ast.h"
#include "coru_token.h"
#include "print.h"

typedef struct coru_ast_code_t coru_ast_code_t;
typedef struct coru_ast_tab_t coru_ast_tab_t;
typedef struct coru_ast_backslash_t coru_ast_backslash_t;
typedef struct coru_ast_string_t coru_ast_string_t;

static coru_ast_code_t * _coru_ast_code_new(void);
static void _coru_ast_code_delete(void *self);
static coru_ast_tab_t * _coru_ast_tab_new(void);
static void _coru_ast_tab_delete(void *self);
static coru_ast_backslash_t * _coru_ast_backslash_new(void);
static void _coru_ast_backslash_delete(void *self);
static coru_ast_string_t * _coru_ast_string_new(void);
static void _coru_ast_string_delete(void *self);

struct coru_ast_t {
    CORU_AST_TYPE ast_t;
    union {
        coru_ast_code_t *code_t;
        coru_ast_tab_t *tab_t;
        coru_ast_backslash_t *backslash_t;
        coru_ast_string_t *string_t;
    } ast;
};

static BOOL _is_valid_ast_type(CORU_AST_TYPE ast_t);

coru_ast_t * coru_ast_new(CORU_AST_TYPE ast_t)
{
    if (!_is_valid_ast_type(ast_t)) {
#if DEBUG
        DEBUG_INFO("Invalid Coru AST type");
#endif
        return NULL;
    }

    coru_ast_t *ast = \
        (coru_ast_t *) malloc(sizeof(coru_ast_t *));
    if (!ast) {
        PUTERR("Failed to allocate memory for coru ast");
        PUTERR("Check available system memory");
        return ast;
    }

    ast->ast_t = ast_t;

    if (CORU_AST_CODE == ast->ast_t) {
        ast->ast.code_t = _coru_ast_code_new();
        if (!(ast->ast.code_t)) {
            free(ast);
            return NULL;
        }
    }
    else if (CORU_AST_TAB == ast->ast_t) {
        ast->ast.tab_t = _coru_ast_tab_new();
        if (!(ast->ast.tab_t)) {
            free(ast);
            return NULL;
        }
    }
    else if (CORU_AST_BACKSLASH == ast->ast_t) {
        ast->ast.backslash_t = _coru_ast_backslash_new();
        if (!(ast->ast.backslash_t)) {
            free(ast);
            return NULL;
        }
    }
    else if (CORU_AST_STRING == ast->ast_t) {
        ast->ast.string_t = _coru_ast_string_new();
        if (!(ast->ast.string_t)) {
            free(ast);
            return NULL;
        }
    }

    return ast;
}

static BOOL _is_valid_ast_type(CORU_AST_TYPE ast_t)
{
    return CORU_AST_CODE == ast_t
        || CORU_AST_TAB == ast_t
        || CORU_AST_BACKSLASH == ast_t
        || CORU_AST_STRING == ast_t;
}

void coru_ast_delete(void *self)
{
    if (!self)
        return;

    CORU_AST_TYPE ast_t = ((coru_ast_t *) self)->ast_t;

    if (CORU_AST_CODE == ast_t) {
        coru_ast_code_t *ast = \
            ((coru_ast_t *) self)->ast.code_t;
        _coru_ast_code_delete(ast);
    }
    else if (CORU_AST_TAB == ast_t) {
        coru_ast_tab_t *ast = \
            ((coru_ast_t *) self)->ast.tab_t;
        _coru_ast_tab_delete(ast);
    }
    else if (CORU_AST_BACKSLASH == ast_t) {
        coru_ast_backslash_t *ast = \
            ((coru_ast_t *) self)->ast.backslash_t;
        _coru_ast_backslash_delete(ast);
    }
    else if (CORU_AST_STRING == ast_t) {
        coru_ast_string_t *ast = \
            ((coru_ast_t *) self)->ast.string_t;
        _coru_ast_string_delete(ast);
    }

    free(self);
}

struct coru_ast_code_t {
    size_t size;
    size_t capacity;
    coru_token_t **tokens;
};

static coru_ast_code_t * _coru_ast_code_new(void)
{
    coru_ast_code_t *ast = \
        (coru_ast_code_t *) malloc(sizeof(coru_ast_code_t));
    if (!ast) {
        PERROR("Failed to allocate memory for coru ast");
        PERROR("Check available system memory");
        return ast;
    }

    ast->size = 0;
    ast->capacity = 8;

    ast->tokens = \
        (coru_token_t **) \
        malloc(ast->capacity * sizeof(coru_token_t *));
    if (!(ast->tokens)) {
        PERROR("Failed to allocate memory for tokens in coru ast");
        PERROR("Check available system memory");
        free(ast);
        return NULL;
    }

    return ast;
}

static void _coru_ast_code_delete(void *self)
{
    if (!self)
        return;

    size_t size = ((coru_ast_code_t *) self)->size;
    coru_token_t **tokens = ((coru_ast_code_t *) self)->tokens;

    {
        size_t i;
        for (i = 0; i < size; i++)
            if (tokens[i])
                coru_token_delete(tokens[i]);
    }

    free(tokens);
    free(self);
}

struct coru_ast_tab_t {
    size_t size;
    size_t capacity;
    coru_token_t *token;
};

static coru_ast_tab_t * _coru_ast_tab_new(void)
{
    coru_ast_tab_t *ast = \
        (coru_ast_tab_t *) malloc(sizeof(coru_ast_tab_t));
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

static void _coru_ast_tab_delete(void *self)
{
    if (!self)
        return;

    coru_token_t *token = ((coru_ast_tab_t *) self)->token;

    free(token);
    free(self);
}

struct coru_ast_backslash_t {
    size_t size;
    size_t capacity;
    coru_token_t *token;
};

static coru_ast_backslash_t * _coru_ast_backslash_new(void)
{
    coru_ast_backslash_t *ast = \
        (coru_ast_backslash_t *) malloc(sizeof(coru_ast_backslash_t));
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

static void _coru_ast_backslash_delete(void *self)
{
    if (!self)
        return;

    coru_token_t *token = ((coru_ast_tab_t *) self)->token;

    free(token);
    free(self);
}

struct coru_ast_string_t {
    size_t size;
    size_t capacity;
    coru_token_t **tokens;
};

static coru_ast_string_t * _coru_ast_string_new(void)
{
    coru_ast_string_t *ast = \
        (coru_ast_string_t *) malloc(sizeof(coru_ast_string_t));
    if (!ast) {
        PERROR("Failed to allocate memory for coru ast");
        PERROR("Check available system memory");
        return ast;
    }

    ast->size = 0;
    ast->capacity = 8;

    ast->tokens = \
        (coru_token_t **) \
        malloc(ast->capacity * sizeof(coru_token_t *));
    if (!(ast->tokens)) {
        PERROR("Failed to allocate memory for tokens in coru ast");
        PERROR("Check available system memory");
        free(ast);
        return NULL;
    }

    return ast;
}

static void _coru_ast_string_delete(void *self)
{
    if (!self)
        return;

    size_t size = ((coru_ast_code_t *) self)->size;
    coru_token_t **tokens = ((coru_ast_code_t *) self)->tokens;

    {
        size_t i;
        for (i = 0; i < size; i++)
            if (tokens[i])
                coru_token_delete(tokens[i]);
    }

    free(tokens);
    free(self);
}
