#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "coru.h"
#include "coru_lexer.h"
#include "coru_token.h"
#include "cstring.h"
#include "print_function.h"

struct coru_lexer_t {
    size_t size;
    size_t capacity;
    size_t index;
    coru_token_t **tokens;
};

coru_lexer_t * coru_lexer_new(void)
{
    coru_lexer_t *lexer = (coru_lexer_t *) malloc(sizeof(coru_lexer_t));
    if (!lexer) {
        PUTERR("Failed to allocate memory for coru lexer object");
        PUTERR("Check available system memory");
        return lexer;
    }

    lexer->size = 0;
    lexer->capacity = 16;
    lexer->index = 0;

    lexer->tokens = \
        (coru_token_t **) malloc(lexer->capacity * sizeof(coru_token_t *));
    if (!(lexer->tokens)) {
        PUTERR("Failed to allocate memory for tokens in coru lexer");
        PUTERR("Check available system memory");
        free(lexer);
        lexer = NULL;
        return lexer;
    }

    {
        size_t i;
        for (i = 0; i < lexer->capacity; i++)
            lexer->tokens[i] = NULL;
    }

    return lexer;
}

static BOOL _coru_lexer_push(coru_lexer_t *self, coru_token_t *token);
static BOOL _is_common_code(char c);

BOOL coru_lexer_lex(coru_lexer_t *self, char *input)
{
    assert(self);

    {
        size_t i;
        for (i = 0; i < strlen(input); i++) {
            if (' ' == input[i]) {
                size_t j = i;

                while (input[j] && ' ' == input[j])
                    j++;

                char *spaces = string_allocate_substring(input, i, j);
                if (!spaces)
                    return FALSE;

                coru_token_t *token = \
                    coru_token_new(CORU_TOKEN_SPACE, spaces);
                if (!token) {
                    free(spaces);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;

                i = j;  /* Update i */
            }
            else if ('\t' == input[i]) {
                char *tab = string_allocate("\t");
                if (!tab)
                    return FALSE;

                coru_token_t *token = coru_token_new(CORU_TOKEN_TAB, tab);
                if (!token) {
                    free(tab);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;
            }
            else if ('\'' == input[i]) {
                char *quote = string_allocate("'");
                if (!quote)
                    return FALSE;

                coru_token_t *token = \
                    coru_token_new(CORU_TOKEN_SINGLE_QUOTE, quote);
                if (!token) {
                    free(quote);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;
            }
            else if ('"' == input[i]) {
                char *quote = string_allocate("\"");
                if (!quote)
                    return FALSE;

                coru_token_t *token = \
                    coru_token_new(CORU_TOKEN_DOUBLE_QUOTE, quote);
                if (!token) {
                    free(quote);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;
            }
            else if ('\\' == input[i]) {
                char *backslash = string_allocate("\\");
                if (!backslash)
                    return FALSE;

                coru_token_t *token = \
                    coru_token_new(CORU_TOKEN_BACKSLASH, backslash);
                if (!token) {
                    free(backslash);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;
            }
            else {
                size_t j = i;

                while (input[j] && _is_common_code(input[j]))
                    j++;

                char *code = string_allocate_substring(input, i, j);
                if (!code)
                    return FALSE;

                coru_token_t *token = \
                    coru_token_new(CORU_TOKEN_CODE, code);
                if (!token) {
                    free(code);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;

                i = j;  /* Update i */
            }
        }
    }

    return TRUE;
}

static BOOL _coru_lexer_expand(coru_lexer_t *self);

static BOOL _coru_lexer_push(coru_lexer_t *self, coru_token_t *token)
{
    assert(self);

    if (!_coru_lexer_expand(self))
        return FALSE;

    if (0 == self->size) {
        self->tokens[self->size] = token;
        self->size += 1;
    }
    else {
        self->size += 1;
        self->tokens[self->size] = token;
    }

    return TRUE;
}

static BOOL _coru_lexer_expand(coru_lexer_t *self)
{
    if (self->size < self->capacity)
        return TRUE;

    self->capacity <<= 1;
    coru_token_t **old_tokens = self->tokens;
    coru_token_t **new_tokens = \
        (coru_token_t **) malloc(self->capacity * sizeof(coru_token_t *));
    if (!(new_tokens)) {
        PUTERR("Failed to allocate memory for coru tokens");
        PUTERR("Check available system memory");
        return FALSE;
    }

    size_t i = 0;
    while (i < self->size) {
        new_tokens[i] = old_tokens[i];
        i++;
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

static BOOL _is_common_code(char c)
{
    return ' ' != c   /* Space */
        && '\t' != c  /* TAB */
        && '\\' != c  /* Backslash */
        && '\'' != c  /* Single quote */
        && '"' != c;  /* Double quote */
}

coru_token_t * coru_lexer_next(coru_lexer_t *self)
{
    assert(self);

    if (self->index >= self->size)
        return NULL;

    coru_token_t *token = coru_token_copy(self->tokens[self->index]);
    if (!token)
        return NULL;

    self->index += 1;

    return token;
}

coru_token_t * coru_lexer_peek(coru_lexer_t *self, size_t n)
{
    assert(self);

    if (self->index + n >= self->size)
        return NULL;

    coru_token_t *token = coru_token_copy(self->tokens[self->index]);
    if (!token)
        return NULL;

    return token;
}

void coru_lexer_delete(void *self)
{
    if (!self)
        return;

    coru_token_t **tokens = ((coru_lexer_t *) self)->tokens;

    {
        size_t size = ((coru_lexer_t *) self)->capacity;
        size_t i;
        for (i = 0; i < size; i++) {
            if (tokens[i])
                coru_token_delete(tokens[i]);
        }
    }

    free(tokens);
    free(self);
}
