#include <assert.h>
#include <stdlib.h>
#include "coru.h"
#include "coru_ast.h"
#include "coru_token.h"
#include "print.h"

typedef struct coru_ast_code_t coru_ast_code_t;
typedef struct coru_ast_tab_t coru_ast_tab_t;
typedef struct coru_ast_backslash_t coru_ast_backslash_t;
typedef struct coru_ast_ampersand_t coru_ast_ampersand_t;
typedef struct coru_ast_backtick_t coru_ast_backtick_t;
typedef struct coru_ast_string_t coru_ast_string_t;

static coru_ast_code_t * _coru_ast_code_new(void);
static void _coru_ast_code_delete(void *self);
static BOOL _coru_ast_code_add(coru_ast_code_t *self, coru_token_t *token);

static coru_ast_tab_t * _coru_ast_tab_new(void);
static void _coru_ast_tab_delete(void *self);
static BOOL _coru_ast_tab_add(coru_ast_tab_t *self, coru_token_t *token);

static coru_ast_backslash_t * _coru_ast_backslash_new(void);
static void _coru_ast_backslash_delete(void *self);
static BOOL _coru_ast_backslash_add(
    coru_ast_backslash_t *self, coru_token_t *token);

static coru_ast_ampersand_t * _coru_ast_ampersand_new(void);
static void _coru_ast_ampersand_delete(void *self);
static BOOL _coru_ast_ampersand_add(
    coru_ast_ampersand_t *self, coru_token_t *token);

static coru_ast_backtick_t * _coru_ast_backtick_new(void);
static void _coru_ast_backtick_delete(void *self);
static BOOL _coru_ast_backtick_add(
    coru_ast_backtick_t *self, coru_token_t *token);

static coru_ast_string_t * _coru_ast_string_new(void);
static void _coru_ast_string_delete(void *self);
static BOOL _coru_ast_string_add(coru_ast_string_t *self, coru_token_t *token);


struct coru_ast_t {
    CORU_AST_TYPE ast_t;
    union {
        coru_ast_code_t *code_t;
        coru_ast_tab_t *tab_t;
        coru_ast_backslash_t *backslash_t;
        coru_ast_ampersand_t *ampersand_t;
        coru_ast_backtick_t *backtick_t;
        coru_ast_string_t *string_t;
    } ast;
};

#define _is_valid_ast_type(ast_t) \
    (CORU_AST_CODE == (ast_t) \
        || CORU_AST_TAB == (ast_t) \
        || CORU_AST_BACKSLASH == (ast_t) \
        || CORU_AST_STRING == (ast_t) \
        || CORU_AST_AMPERSAND == (ast_t) \
        || CORU_AST_BACKTICK == (ast_t))

coru_ast_t * coru_ast_new(CORU_AST_TYPE ast_t)
{
    if (!_is_valid_ast_type(ast_t)) {
#if DEBUG
        DEBUG_INFO("Invalid Coru AST type");
#endif
        return NULL;
    }

    coru_ast_t *ast = \
        (coru_ast_t *) malloc(sizeof(coru_ast_t));
    if (!ast) {
        PUTERR("Failed to allocate memory for coru ast");
        PUTERR("Check available system memory");
        return ast;
    }

    ast->ast_t = ast_t;

    switch (ast->ast_t) {
    case CORU_AST_CODE:
        ast->ast.code_t = _coru_ast_code_new();
        if (!(ast->ast.code_t)) {
            free(ast);
            return NULL;
        }
        break;
    case CORU_AST_TAB:
        ast->ast.tab_t = _coru_ast_tab_new();
        if (!(ast->ast.tab_t)) {
            free(ast);
            return NULL;
        }
        break;
    case CORU_AST_BACKSLASH:
        ast->ast.backslash_t = _coru_ast_backslash_new();
        if (!(ast->ast.backslash_t)) {
            free(ast);
            return NULL;
        }
        break;
    case CORU_AST_AMPERSAND:
        ast->ast.ampersand_t = _coru_ast_ampersand_new();
        if (!(ast->ast.ampersand_t)) {
            free(ast);
            return NULL;
        }
        break;
    case CORU_AST_BACKTICK:
        ast->ast.backtick_t = _coru_ast_backtick_new();
        if (!(ast->ast.backtick_t)) {
            free(ast);
            return NULL;
        }
        break;
    case CORU_AST_STRING:
        ast->ast.string_t = _coru_ast_string_new();
        if (!(ast->ast.string_t)) {
            free(ast);
            return NULL;
        }
        break;
    }

    return ast;
}

void coru_ast_delete(void *self)
{
    if (!self)
        return;

    CORU_AST_TYPE ast_t = ((coru_ast_t *) self)->ast_t;

    switch (ast_t) {
    case CORU_AST_CODE:
        {
            coru_ast_code_t *ast = \
                ((coru_ast_t *) self)->ast.code_t;
            _coru_ast_code_delete(ast);   
        }
        break;
    case CORU_AST_TAB:
        {
            coru_ast_tab_t *ast = \
                ((coru_ast_t *) self)->ast.tab_t;
            _coru_ast_tab_delete(ast);
        }
        break;
    case CORU_AST_BACKSLASH:
        {
            coru_ast_backslash_t *ast = \
                ((coru_ast_t *) self)->ast.backslash_t;
            _coru_ast_backslash_delete(ast);
        }
        break;
    case CORU_AST_AMPERSAND:
        {
            coru_ast_ampersand_t *ast = \
                ((coru_ast_t *) self)->ast.ampersand_t;
            _coru_ast_ampersand_delete(ast);
        }
        break;
    case CORU_AST_BACKTICK:
        {
            coru_ast_backtick_t *ast = \
                ((coru_ast_t *) self)->ast.backtick_t;
            _coru_ast_backtick_delete(ast);
        }
        break;
    case CORU_AST_STRING:
        {
            coru_ast_string_t *ast = \
                ((coru_ast_t *) self)->ast.string_t;
            _coru_ast_string_delete(ast);
        }
        break;
    }

    free(self);
}

BOOL coru_ast_add(coru_ast_t *self, coru_token_t *token)
{
    assert(self);
    assert(token);

    BOOL added = FALSE;

    switch (self->ast_t) {
    case CORU_AST_CODE:
        added = _coru_ast_code_add(self->ast.code_t, token);
        break;
    case CORU_AST_TAB:
        added = _coru_ast_tab_add(self->ast.tab_t, token);
        break;
    case CORU_AST_BACKSLASH:
        added = _coru_ast_backslash_add(self->ast.backslash_t, token);
        break;
    case CORU_AST_AMPERSAND:
        added = _coru_ast_ampersand_add(self->ast.ampersand_t, token);
        break;
    case CORU_AST_BACKTICK:
        added = _coru_ast_backtick_add(self->ast.backtick_t, token);
        break;
    case CORU_AST_STRING:
        added = _coru_ast_string_add(self->ast.string_t, token);
        break;
    }

    return added;
}

CORU_AST_TYPE coru_ast_type(coru_ast_t *self)
{
    assert(self);

    return self->ast_t;
}


/* Implement coru_ast_code_t */
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

    {
        size_t i;
        for (i = 0; i < ast->capacity; i++)
            ast->tokens[i] = NULL;
    }

    return ast;
}

static void _coru_ast_code_delete(void *self)
{
    if (!self)
        return;

    size_t size = ((coru_ast_code_t *) self)->capacity;
    coru_token_t **tokens = ((coru_ast_code_t *) self)->tokens;

    {
        size_t i;
        for (i = 0; i < size; i++) {
            if (tokens[i])
                coru_token_delete(tokens[i]);
        }
    }

    free(tokens);
    free(self);
}

static BOOL _coru_ast_code_expand(coru_ast_code_t *self);

static BOOL _coru_ast_code_add(coru_ast_code_t *self, coru_token_t *token)
{
    if (!_coru_ast_code_expand(self))
        return FALSE;

    self->tokens[self->size] = token;
    self->size += 1;

    return TRUE;
}

static BOOL _coru_ast_code_expand(coru_ast_code_t *self)
{
    if (self->size < self->capacity)
        return TRUE;

    self->capacity <<= 1;
    coru_token_t **old_tokens = self->tokens;
    coru_token_t **new_tokens = \
        (coru_token_t **) \
        malloc(self->capacity * sizeof(coru_token_t *));
    if (!new_tokens) {
        PUTERR("Failed to allocate memory for tokens in coru ast");
        PUTERR("Check available system memory");
        return FALSE;
    }

    {
        size_t i;
        for (i = 0; i < self->size; i++)
            new_tokens[i] = old_tokens[i];
    }

    {
        size_t i;
        for (i = self->size; i < self->capacity; i++)
            new_tokens[i] = NULL;
    }

    self->tokens = new_tokens;
    free(old_tokens);

    return TRUE;
}


/* Implement coru_ast_tab_t */
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

    coru_token_delete(token);
    free(self);
}

static BOOL _coru_ast_tab_add(coru_ast_tab_t *self, coru_token_t *token)
{
    assert(self);

    if (self->size >= self->capacity)
        return FALSE;

    self->token = token;
    self->size += 1;

    return TRUE;
}


/* Implement coru_ast_backslash_t */
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

    coru_token_t *token = ((coru_ast_backslash_t *) self)->token;

    coru_token_delete(token);
    free(self);
}

static BOOL _coru_ast_backslash_add(
    coru_ast_backslash_t *self, coru_token_t *token)
{
    if (self->size >= self->capacity)
        return FALSE;

    self->token = token;
    self->size += 1;

    return TRUE;
}


/* Implement coru_ast_ampersand_t */
struct coru_ast_ampersand_t {
    size_t size;
    size_t capacity;
    coru_token_t *token;
};

static coru_ast_ampersand_t * _coru_ast_ampersand_new(void)
{
    coru_ast_ampersand_t *ast = \
        (coru_ast_ampersand_t *) malloc(sizeof(coru_ast_ampersand_t));
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

static void _coru_ast_ampersand_delete(void *self)
{
    if (!self)
        return;

    coru_token_t *token = ((coru_ast_ampersand_t *) self)->token;

    coru_token_delete(token);
    free(self);
}

static BOOL _coru_ast_ampersand_add(
    coru_ast_ampersand_t *self, coru_token_t *token)
{
    if (self->size >= self->capacity)
        return FALSE;

    self->token = token;
    self->size += 1;

    return TRUE;
}

/* Implement coru_ast_backtick_t */
struct coru_ast_backtick_t {
    size_t size;
    size_t capacity;
    coru_token_t *token;
};

static coru_ast_backtick_t * _coru_ast_backtick_new(void)
{
    coru_ast_backtick_t *ast = \
        (coru_ast_backtick_t *) malloc(sizeof(coru_ast_backtick_t));
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

static void _coru_ast_backtick_delete(void *self)
{
    if (!self)
        return;

    coru_token_t *token = ((coru_ast_backtick_t *) self)->token;

    coru_token_delete(token);
    free(self);
}

static BOOL _coru_ast_backtick_add(
    coru_ast_backtick_t *self, coru_token_t *token)
{
    if (self->size >= self->capacity)
        return FALSE;

    self->token = token;
    self->size += 1;

    return TRUE;
}

/* Implement coru_ast_string_t */
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

    {
        size_t i;
        for (i = 0; i < ast->capacity; i++)
            ast->tokens[i] = NULL;
    }

    return ast;
}

static void _coru_ast_string_delete(void *self)
{
    if (!self)
        return;

    size_t size = ((coru_ast_code_t *) self)->capacity;
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

static BOOL _coru_ast_string_expand(coru_ast_string_t *self);

static BOOL _coru_ast_string_add(coru_ast_string_t *self, coru_token_t *token)
{
    if (!_coru_ast_string_expand(self))
        return FALSE;

    self->tokens[self->size] = token;
    self->size += 1;

    return TRUE;
}

static BOOL _coru_ast_string_expand(coru_ast_string_t *self)
{
    if (self->size < self->capacity)
        return TRUE;

    self->capacity <<= 1;
    coru_token_t **old_tokens = self->tokens;
    coru_token_t **new_tokens = \
        (coru_token_t **) \
        malloc(self->capacity * sizeof(coru_token_t *));
    if (!new_tokens) {
        PUTERR("Failed to allocate memory for tokens in coru ast");
        PUTERR("Check available system memory");
        return FALSE;
    }

    {
        size_t i;
        for (i = 0; i < self->size; i++)
            new_tokens[i] = old_tokens[i];
    }

    {
        size_t i;
        for (i = self->size; i < self->capacity; i++)
            new_tokens[i] = NULL;
    }

    self->tokens = new_tokens;
    free(old_tokens);

    return TRUE;
}
