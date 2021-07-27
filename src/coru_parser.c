#include <assert.h>
#include <stdlib.h>
#include "coru_ast.h"
#include "coru_parser.h"
#include "coru_token.h"
#include "print.h"

struct coru_parser_t {
    size_t size;
    size_t capacity;
    size_t index;
    coru_ast_t **asts;
};

coru_parser_t * coru_parser_new(void)
{
    coru_parser_t *parser = \
        (coru_parser_t *) malloc(sizeof(coru_parser_t));
    if (!parser) {
        PUTERR("Failed to allocate memory for Coru Parser");
        PUTERR("Check available system memory");
        return parser;
    }

    parser->size = 0;
    parser->capacity = 16;
    parser->index = 0;

    parser->asts = \
        (coru_ast_t **) \
        malloc(parser->capacity * sizeof(coru_ast_t *));
    if (!(parser->asts)) {
        PUTERR("Failed to allocate memory for internal ast array of Coru Parser");
        PUTERR("Check available system memory");
        free(parser);
        return NULL;
    }

    {
        size_t i;
        for (i = 0; i < parser->capacity; i++)
            parser->asts[i] = NULL;
    }

    return parser;
}

#define IS_CODE_TOKEN(t) \
    (CORU_TOKEN_SINGLE_QUOTE != (t) \
     && CORU_TOKEN_DOUBLE_QUOTE != (t) \
     && CORU_TOKEN_TAB != (t) \
     && CORU_TOKEN_BACKSLASH != (t) \
     && CORU_TOKEN_AMPERSAND != (t) \
     && CORU_TOKEN_BACKTICK != (t))

static BOOL _coru_parser_expand(coru_parser_t *self);

BOOL coru_parser_parse(coru_parser_t *self, coru_lexer_t *lexer)
{
    assert(self);
    assert(lexer);

    coru_token_t *token = coru_lexer_next(lexer);
    /* Scan the tokens with a finite automata. */
    while (token) {
        if (!_coru_parser_expand(self))
            return FALSE;

        coru_ast_t *ast = NULL;

        if (token && CORU_TOKEN_SINGLE_QUOTE == coru_token_type(token)) {
            #if DEBUG
                PUTERR("Transform single quote token: (%d) -->%s<--",
                    coru_token_type(token), coru_token_text(token));
            #endif

            ast = coru_ast_new(CORU_AST_STRING);
            if (!ast)
                return FALSE;

            /* Consume the starting single quote token. */
            if (!coru_ast_add(ast, token))
                return FALSE;

            token = coru_lexer_next(lexer);

            /* Scan the tokens to parse the rest of a single-quoted string. */
            while (token) {
                if (!_coru_parser_expand(self))
                    return FALSE;

                if (token && CORU_TOKEN_BACKSLASH == coru_token_type(token)) {
                    /* Consume the backslash token. */
                    if (!coru_ast_add(ast, token))
                        return FALSE;

                    /* Consume one extra token. */
                    token = coru_lexer_next(lexer);
                    if (token) {
                        if (!coru_ast_add(ast, token))
                            return FALSE;
                    }
                }
                else if (token && CORU_TOKEN_SINGLE_QUOTE == coru_token_type(token)) {
                    /* Consume the ending single quote token. */
                    if (!coru_ast_add(ast, token))
                        return FALSE;

                    token = coru_lexer_next(lexer);

                    /* Stop the finite automata. */
                    break;
                }
                else {
                    /* Consume any text token within this string. */
                    if (token) {
                        if (!coru_ast_add(ast, token))
                            return FALSE;
                    }
                }

                token = coru_lexer_next(lexer);
            }
        }
        else if (token && CORU_TOKEN_DOUBLE_QUOTE == coru_token_type(token)) {
            #if DEBUG
                PUTERR("Transform double quote token: (%d) -->%s<--",
                    coru_token_type(token), coru_token_text(token));
            #endif

            ast = coru_ast_new(CORU_AST_STRING);
            if (!ast)
                return FALSE;

            /* Consume the starting double quote token. */
            if (!coru_ast_add(ast, token))
                return FALSE;

            token = coru_lexer_next(lexer);

            /* Scan the tokens to parse the rest of a double-quoted string. */
            while (token) {
                if (!_coru_parser_expand(self))
                    return FALSE;

                if (token && CORU_TOKEN_BACKSLASH == coru_token_type(token)) {
                    /* Consume the backslash token. */
                    if (!coru_ast_add(ast, token))
                        return FALSE;

                    /* Consume one extra token. */
                    token = coru_lexer_next(lexer);
                    if (token) {
                        if (!coru_ast_add(ast, token))
                            return FALSE;
                    }
                }
                else if (token && CORU_TOKEN_DOUBLE_QUOTE == coru_token_type(token)) {
                    /* Consume the ending double quote token. */
                    if (!coru_ast_add(ast, token))
                        return FALSE;

                    token = coru_lexer_next(lexer);

                    /* Stop the finite automata. */
                    break;
                }
                else {
                    /* Consume any text token within this string. */
                    if (token) {
                        if (!coru_ast_add(ast, token))
                            return FALSE;
                    }
                }

                token = coru_lexer_next(lexer);
            }
        }
        else if (token && CORU_TOKEN_BACKSLASH == coru_token_type(token)) {
            #if DEBUG
                PUTERR("Transform backslash token: (%d) -->%s<--",
                    coru_token_type(token), coru_token_text(token));
            #endif
            ast = coru_ast_new(CORU_AST_BACKSLASH);
            if (!ast)
                return FALSE;

            if (!coru_ast_add(ast, token))
                return FALSE;

            token = coru_lexer_next(lexer);
        }
        else if (token && CORU_TOKEN_AMPERSAND == coru_token_type(token)) {
            #if DEBUG
                PUTERR("Transform ampersand token: (%d) -->%s<--",
                    coru_token_type(token), coru_token_text(token));
            #endif
            ast = coru_ast_new(CORU_AST_AMPERSAND);
            if (!ast)
                return FALSE;

            if (!coru_ast_add(ast, token))
                return FALSE;

            token = coru_lexer_next(lexer);
        }
        else if (token && CORU_TOKEN_BACKTICK == coru_token_type(token)) {
            #if DEBUG
                PUTERR("Transform backtoken token: (%d) -->%s<--",
                    coru_token_type(token), coru_token_text(token));
            #endif
            ast = coru_ast_new(CORU_AST_BACKTICK);
            if (!ast)
                return FALSE;

            if (!coru_ast_add(ast, token))
                return FALSE;

            token = coru_lexer_next(lexer);
        }
        else if (token && CORU_TOKEN_TAB == coru_token_type(token)) {
            #if DEBUG
                PUTERR("Transform TAB token: (%d) -->%s<--",
                    coru_token_type(token), coru_token_text(token));
            #endif
            ast = coru_ast_new(CORU_AST_TAB);
            if (!ast)
                return FALSE;

            if (!coru_ast_add(ast, token))
                return FALSE;

            token = coru_lexer_next(lexer);
        }
        else if (token && IS_CODE_TOKEN(coru_token_type(token))) {
        #if DEBUG
            PUTERR("Pass code token: (%d) -->%s<--",
                coru_token_type(token), coru_token_text(token));
        #endif
            /* Refactor it later. */
            coru_token_delete(token);
            token = coru_lexer_next(lexer);  /* Pass. */
        }
        else {
            #if DEBUG
                if (token)
                    PUTERR("Pass other token: (%d) -->%s<--",
                        coru_token_type(token), coru_token_text(token));
            #endif

            coru_token_delete(token);  /* Pass. */
            token = coru_lexer_next(lexer);
        }

        if (ast) {
            self->asts[self->size] = ast;
            self->size += 1;
        }
    }

    return TRUE;
}

static BOOL _coru_parser_expand(coru_parser_t *self)
{
    if (self->size < self->capacity)
        return TRUE;

    self->capacity <<= 1;
    coru_ast_t **old_asts = self->asts;
    coru_ast_t **new_asts = \
        (coru_ast_t **) \
        malloc(self->capacity * sizeof(coru_ast_t *));
    if (!new_asts)
        return FALSE;

    {
        size_t i = 0;
        while (i < self->size) {
            new_asts[i] = old_asts[i];
            i++;
        }
    }

    {
        size_t i;
        for (i = self->size; i < self->capacity; i++)
            new_asts[i] = NULL;
    }

    self->asts = new_asts;
    free(old_asts);

    return TRUE;
}

coru_ast_t * coru_parser_next(coru_parser_t *self)
{
    assert(self);

    if (self->index >= self->size)
        return NULL;

    coru_ast_t *ast = self->asts[self->index];
    if (!ast)
        return NULL;

    self->index += 1;

    return ast;
}

void coru_parser_delete(void *self)
{
    if (!self)
        return;

    coru_ast_t **asts = ((coru_parser_t *) self)->asts;

    {
        size_t size = ((coru_parser_t *) self)->capacity;
        size_t i;
        for (i = 0; i < size; i++) {
            if (asts && asts[i])
                coru_ast_delete(asts[i]);
        }
    }

    free(asts);
    free(self);
}
