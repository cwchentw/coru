#include <assert.h>
#include <stdlib.h>
#include "print.h"
#include "cstring.h"
#include "uncoru.h"
#include "uncoru_ast.h"
#include "uncoru_token.h"

typedef struct uncoru_ast_code_t uncoru_ast_code_t;
typedef struct uncoru_ast_space_t uncoru_ast_space_t;
typedef struct uncoru_ast_tab_t uncoru_ast_tab_t;
typedef struct uncoru_ast_backslash_t uncoru_ast_backslash_t;
typedef struct uncoru_ast_ampersand_t uncoru_ast_ampersand_t;
typedef struct uncoru_ast_line_number_t uncoru_ast_line_number_t;
typedef struct uncoru_ast_string_t uncoru_ast_string_t;

static uncoru_ast_code_t * _uncoru_ast_code_new(void);
static void _uncoru_ast_code_delete(void *self);
static BOOL _uncoru_ast_code_add(uncoru_ast_code_t *self, uncoru_token_t *token);
static char * _uncoru_ast_code_text(uncoru_ast_code_t *self);

static uncoru_ast_space_t * _uncoru_ast_space_new(void);
static void _uncoru_ast_space_delete(void *self);
static BOOL _uncoru_ast_space_add(uncoru_ast_space_t *self, uncoru_token_t *token);
static char * _uncoru_ast_space_text(uncoru_ast_space_t *self);

static uncoru_ast_tab_t * _uncoru_ast_tab_new(void);
static void _uncoru_ast_tab_delete(void *self);
static BOOL _uncoru_ast_tab_add(uncoru_ast_tab_t *self, uncoru_token_t *token);
static char * _uncoru_ast_tab_text(uncoru_ast_tab_t *self);

static uncoru_ast_backslash_t * _uncoru_ast_backslash_new(void);
static void _uncoru_ast_backslash_delete(void *self);
static BOOL _uncoru_ast_backslash_add(uncoru_ast_backslash_t *self, uncoru_token_t *token);
static char * _uncoru_ast_backslash_text(uncoru_ast_backslash_t *self);

static uncoru_ast_ampersand_t * _uncoru_ast_ampersand_new(void);
static void _uncoru_ast_ampersand_delete(void *self);
static BOOL _uncoru_ast_ampersand_add(uncoru_ast_ampersand_t *self, uncoru_token_t *token);
static char * _uncoru_ast_ampersand_text(uncoru_ast_ampersand_t *self);

static uncoru_ast_line_number_t * _uncoru_ast_line_number_new(void);
static void _uncoru_ast_line_number_delete(void *self);
static BOOL _uncoru_ast_line_number_add(uncoru_ast_line_number_t *self, uncoru_token_t *token);
static char * _uncoru_ast_line_number_text(uncoru_ast_line_number_t *self);

static uncoru_ast_string_t * _uncoru_ast_string_new(void);
static void _uncoru_ast_string_delete(void *self);
static BOOL _uncoru_ast_string_add(uncoru_ast_string_t *self, uncoru_token_t *token);
static char * _uncoru_ast_string_text(uncoru_ast_string_t *self);


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
    else if (UNCORU_AST_STRING == ast->ast_t) {
        ast->ast.string_t = _uncoru_ast_string_new();
        if (!(ast->ast.string_t)) {
            free(ast);
            return NULL;
        }
    }
    else if (UNCORU_AST_LINE_NUNBER == ast->ast_t) {
        ast->ast.line_number_t = _uncoru_ast_line_number_new();
        if (!(ast->ast.line_number_t)) {
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
    else if (UNCORU_AST_BACKSLASH == ast->ast_t) {
        ast->ast.backslash_t = _uncoru_ast_backslash_new();
        if (!(ast->ast.backslash_t)) {
            free(ast);
            return NULL;
        }
    }
    else if (UNCORU_AST_AMPERSAND == ast->ast_t) {
        ast->ast.ampersand_t = _uncoru_ast_ampersand_new();
        if (!(ast->ast.ampersand_t)) {
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
    else if (UNCORU_AST_STRING == ast_t) {
        uncoru_ast_string_t *ast = \
            ((uncoru_ast_t *) self)->ast.string_t;
        _uncoru_ast_string_delete(ast);
    }
    else if (UNCORU_AST_LINE_NUNBER == ast_t) {
        uncoru_ast_line_number_t *ast = \
            ((uncoru_ast_t *) self)->ast.line_number_t;
        _uncoru_ast_line_number_delete(ast);
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
    else if (UNCORU_AST_BACKSLASH == ast_t) {
        uncoru_ast_backslash_t *ast = \
            ((uncoru_ast_t *) self)->ast.backslash_t;
        _uncoru_ast_backslash_delete(ast);
    }
    else if (UNCORU_AST_AMPERSAND == ast_t) {
        uncoru_ast_ampersand_t *ast = \
            ((uncoru_ast_t *) self)->ast.ampersand_t;
        _uncoru_ast_ampersand_delete(ast);
    }

    free(self);
}

BOOL uncoru_ast_add(uncoru_ast_t *self, uncoru_token_t *token)
{
    assert(self);
    assert(token);

    BOOL added = FALSE;

    if (UNCORU_AST_CODE == self->ast_t)
        added = _uncoru_ast_code_add(self->ast.code_t, token);
    else if (UNCORU_AST_STRING == self->ast_t)
        added = _uncoru_ast_string_add(self->ast.string_t, token);
    else if (UNCORU_AST_LINE_NUNBER == self->ast_t)
        added = _uncoru_ast_line_number_add(self->ast.line_number_t, token);
    else if (UNCORU_AST_SPACE == self->ast_t)
        added = _uncoru_ast_space_add(self->ast.space_t, token);
    else if (UNCORU_AST_TAB == self->ast_t)
        added = _uncoru_ast_tab_add(self->ast.tab_t, token);
    else if (UNCORU_AST_BACKSLASH == self->ast_t)
        added = _uncoru_ast_backslash_add(self->ast.backslash_t, token);
    else if (UNCORU_AST_AMPERSAND == self->ast_t)
        added = _uncoru_ast_ampersand_add(self->ast.ampersand_t, token);

    return added;
}

UNCORU_AST_TYPE uncoru_ast_type(uncoru_ast_t *self)
{
    assert(self);

    return self->ast_t;
}

char * uncoru_ast_text(uncoru_ast_t *self)
{
    assert(self);

    char *out = NULL;

    switch (self->ast_t) {
    case UNCORU_AST_CODE:
        out = _uncoru_ast_code_text(self->ast.code_t);
        break;
    case UNCORU_AST_LINE_NUNBER:
        out = _uncoru_ast_line_number_text(self->ast.line_number_t);
        break;
    case UNCORU_AST_SPACE:
        out = _uncoru_ast_space_text(self->ast.space_t);
        break;
    case UNCORU_AST_TAB:
        out = _uncoru_ast_tab_text(self->ast.tab_t);
        break;
    case UNCORU_AST_BACKSLASH:
        out = _uncoru_ast_backslash_text(self->ast.backslash_t);
        break;
    case UNCORU_AST_AMPERSAND:
        out = _uncoru_ast_ampersand_text(self->ast.ampersand_t);
        break;
    case UNCORU_AST_STRING:
        out = _uncoru_ast_string_text(self->ast.string_t);
        break;
    }

    return out;
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

static BOOL _uncoru_ast_code_expand(uncoru_ast_code_t *self);

static BOOL _uncoru_ast_code_add(uncoru_ast_code_t *self, uncoru_token_t *token)
{
    if (!_uncoru_ast_code_expand(self))
        return FALSE;

    self->tokens[self->size] = token;
    self->size += 1;

    return TRUE;
}

static BOOL _uncoru_ast_code_expand(uncoru_ast_code_t *self)
{
    if (self->size < self->capacity)
        return TRUE;

    self->capacity <<= 1;
    uncoru_token_t **old_tokens = self->tokens;
    uncoru_token_t **new_tokens = \
        (uncoru_token_t **) \
        malloc(self->capacity * sizeof(uncoru_token_t *));
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

static char * _uncoru_ast_code_text(uncoru_ast_code_t *self)
{
    assert(self);

    char *out = string_allocate(uncoru_token_text(self->tokens[0]));
    if (!out)
        return out;

    {
        size_t i;
        for (i = 1; i < self->size; ++i) {
            char *a = out;
            char *b = string_allocate(uncoru_token_text(self->tokens[i]));
            if (!b) {
                free(a);
                return NULL;
            }

            out = string_concat(a, b);
            if (!out) {
                free(a);
                free(b);
                return NULL;
            }

            free(a);
            free(b);
        }
    }

    return out;
}


/* Implement uncoru_ast_space_t */
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

static BOOL _uncoru_ast_space_add(uncoru_ast_space_t *self, uncoru_token_t *token)
{
    assert(self);

    if (self->size >= self->capacity)
        return FALSE;

    self->token = token;
    self->size += 1;

    return TRUE;
}

static char * _uncoru_ast_space_text(uncoru_ast_space_t *self)
{
    assert(self);

    char *out = string_allocate(uncoru_token_text(self->token));

    return out;
}


/* Implement uncoru_ast_tab_t */
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

static BOOL _uncoru_ast_tab_add(uncoru_ast_tab_t *self, uncoru_token_t *token)
{
    assert(self);

    if (self->size >= self->capacity)
        return FALSE;

    self->token = token;
    self->size += 1;

    return TRUE;
}

static char * _uncoru_ast_tab_text(uncoru_ast_tab_t *self)
{
    assert(self);

    char *out = string_allocate(uncoru_token_text(self->token));

    return out;
}


/* Implement uncoru_ast_backslash_t */
struct uncoru_ast_backslash_t {
    size_t size;
    size_t capacity;
    uncoru_token_t *token;
};

static uncoru_ast_backslash_t * _uncoru_ast_backslash_new(void)
{
    uncoru_ast_backslash_t *ast = \
        (uncoru_ast_backslash_t *) malloc(sizeof(uncoru_ast_backslash_t));
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

static void _uncoru_ast_backslash_delete(void *self)
{
    if (!self)
        return;

    uncoru_token_t *token = ((uncoru_ast_backslash_t *) self)->token;

    uncoru_token_delete(token);
    free(self);
}

static BOOL _uncoru_ast_backslash_add(uncoru_ast_backslash_t *self, uncoru_token_t *token)
{
    assert(self);

    if (self->size >= self->capacity)
        return FALSE;

    self->token = token;
    self->size += 1;

    return TRUE;
}

static char * _uncoru_ast_backslash_text(uncoru_ast_backslash_t *self)
{
    assert(self);

    char *out = string_allocate(uncoru_token_text(self->token));

    return out;
}


/* Implement uncoru_ast_ampersand_t */
struct uncoru_ast_ampersand_t {
    size_t size;
    size_t capacity;
    uncoru_token_t *token;
};

static uncoru_ast_ampersand_t * _uncoru_ast_ampersand_new(void)
{
    uncoru_ast_ampersand_t *ast = \
        (uncoru_ast_ampersand_t *) malloc(sizeof(uncoru_ast_ampersand_t));
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

static void _uncoru_ast_ampersand_delete(void *self)
{
    if (!self)
        return;

    uncoru_token_t *token = ((uncoru_ast_ampersand_t *) self)->token;

    uncoru_token_delete(token);
    free(self);
}

static BOOL _uncoru_ast_ampersand_add(uncoru_ast_ampersand_t *self, uncoru_token_t *token)
{
    assert(self);

    if (self->size >= self->capacity)
        return FALSE;

    self->token = token;
    self->size += 1;

    return TRUE;
}

static char * _uncoru_ast_ampersand_text(uncoru_ast_ampersand_t *self)
{
    assert(self);

    char *out = string_allocate(uncoru_token_text(self->token));

    return out;
}


/* Implement uncoru_ast_line_number_t */
struct uncoru_ast_line_number_t {
    size_t size;
    size_t capacity;
    uncoru_token_t **tokens;
};

static uncoru_ast_line_number_t * _uncoru_ast_line_number_new(void)
{
    uncoru_ast_line_number_t *ast = \
        (uncoru_ast_line_number_t *) malloc(sizeof(uncoru_ast_line_number_t));
    if (!ast) {
        PERROR("Failed to allocate memory for coru ast");
        PERROR("Check available system memory");
        return ast;
    }

    ast->size = 0;
    ast->capacity = 8;

    ast->tokens = \
        (uncoru_token_t **) \
        malloc(ast->capacity * sizeof(uncoru_token_t *));
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

static void _uncoru_ast_line_number_delete(void *self)
{
    if (!self)
        return;

    size_t size = ((uncoru_ast_line_number_t *) self)->capacity;
    uncoru_token_t **tokens = ((uncoru_ast_line_number_t *) self)->tokens;

    {
        size_t i;
        for (i = 0; i < size; i++)
            if (tokens[i])
                uncoru_token_delete(tokens[i]);
    }

    free(tokens);
    free(self);
}

static BOOL _uncoru_ast_line_number_expand(uncoru_ast_line_number_t *self);

static BOOL _uncoru_ast_line_number_add(uncoru_ast_line_number_t *self, uncoru_token_t *token)
{
    if (!_uncoru_ast_line_number_expand(self))
        return FALSE;

    self->tokens[self->size] = token;
    self->size += 1;

    return TRUE;
}

static BOOL _uncoru_ast_line_number_expand(uncoru_ast_line_number_t *self)
{
    if (self->size < self->capacity)
        return TRUE;

    self->capacity <<= 1;
    uncoru_token_t **old_tokens = self->tokens;
    uncoru_token_t **new_tokens = \
        (uncoru_token_t **) \
        malloc(self->capacity * sizeof(uncoru_token_t *));
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

static char * _uncoru_ast_line_number_text(uncoru_ast_line_number_t *self)
{
    assert(self);

    char *out = string_allocate(uncoru_token_text(self->tokens[0]));
    if (!out)
        return out;

    {
        size_t i;
        for (i = 1; i < self->size; ++i) {
            char *a = out;
            char *b = string_allocate(uncoru_token_text(self->tokens[i]));
            if (!b) {
                free(a);
                return NULL;
            }

            out = string_concat(a, b);
            if (!out) {
                free(a);
                free(b);
                return NULL;
            }

            free(a);
            free(b);
        }
    }

    return out;
}


/* Implement uncoru_ast_string_t */
struct uncoru_ast_string_t {
    size_t size;
    size_t capacity;
    uncoru_token_t **tokens;
};

static uncoru_ast_string_t * _uncoru_ast_string_new(void)
{
    uncoru_ast_string_t *ast = \
        (uncoru_ast_string_t *) malloc(sizeof(uncoru_ast_string_t));
    if (!ast) {
        PERROR("Failed to allocate memory for coru ast");
        PERROR("Check available system memory");
        return ast;
    }

    ast->size = 0;
    ast->capacity = 8;

    ast->tokens = \
        (uncoru_token_t **) \
        malloc(ast->capacity * sizeof(uncoru_token_t *));
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

static void _uncoru_ast_string_delete(void *self)
{
    if (!self)
        return;

    size_t size = ((uncoru_ast_string_t *) self)->capacity;
    uncoru_token_t **tokens = ((uncoru_ast_string_t *) self)->tokens;

    {
        size_t i;
        for (i = 0; i < size; i++)
            if (tokens[i])
                uncoru_token_delete(tokens[i]);
    }

    free(tokens);
    free(self);
}

static BOOL _uncoru_ast_string_expand(uncoru_ast_string_t *self);

static BOOL _uncoru_ast_string_add(uncoru_ast_string_t *self, uncoru_token_t *token)
{
    if (!_uncoru_ast_string_expand(self))
        return FALSE;

    self->tokens[self->size] = token;
    self->size += 1;

    return TRUE;
}

static BOOL _uncoru_ast_string_expand(uncoru_ast_string_t *self)
{
    if (self->size < self->capacity)
        return TRUE;

    self->capacity <<= 1;
    uncoru_token_t **old_tokens = self->tokens;
    uncoru_token_t **new_tokens = \
        (uncoru_token_t **) \
        malloc(self->capacity * sizeof(uncoru_token_t *));
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

static char * _uncoru_ast_string_text(uncoru_ast_string_t *self)
{
    assert(self);

    char *out = string_allocate(uncoru_token_text(self->tokens[0]));
    if (!out)
        return out;

    {
        size_t i;
        for (i = 1; i < self->size; ++i) {
            char *a = out;
            char *b = string_allocate(uncoru_token_text(self->tokens[i]));
            if (!b) {
                free(a);
                return NULL;
            }

            out = string_concat(a, b);
            if (!out) {
                free(a);
                free(b);
                return NULL;
            }

            free(a);
            free(b);
        }
    }

    return out;
}
