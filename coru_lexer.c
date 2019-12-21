#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "coru.h"
#include "coru_lexer.h"
#include "coru_token.h"
#include "cstring.h"
#include "print.h"

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
    lexer->capacity = 2;
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

    #if DEBUG
        PUTERR("Source to scan: -->%s<--", input);
    #endif
    {
        size_t i;
        for (i = 0; i < strlen(input); i++) {
            if (' ' == input[i]) {
                size_t j;

                /* Scan greedily. */
                for (j = i; j < strlen(input); j++) {
                    /* Go one step over last valid position. */
                    if (' ' != input[j])
                        break;
                }

                /* -1: go one step back to last valid position.
                   +1: counting issue.
                 */
                size_t len = j - 1 - i + 1;

                if (len < 1)
                    continue;

                char *spaces = string_allocate_substring(input, i, j - 1);
                if (!spaces)
                    return FALSE;

                #if DEBUG
                    PUTERR("Space as token: -->%s<--", spaces);
                #endif

                coru_token_t *token = \
                    coru_token_new(CORU_TOKEN_SPACE, spaces);
                if (!token) {
                    free(spaces);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;

                i = j - 1;
            }
            else if ('\t' == input[i]) {
                char *tab = string_allocate("\t");
                if (!tab)
                    return FALSE;

                #if DEBUG
                    PUTERR("TAB as token: -->%s<--", tab);
                #endif

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

                #if DEBUG
                    PUTERR("Single quote as token: -->%s<--", quote);
                #endif

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

                #if DEBUG
                    PUTERR("Double quote as token: -->%s<--", quote);
                #endif

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

                #if DEBUG
                    PUTERR("Backslash as token: -->%s<--", backslash);
                #endif

                coru_token_t *token = \
                    coru_token_new(CORU_TOKEN_BACKSLASH, backslash);
                if (!token) {
                    free(backslash);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;
            }
            else if (_is_common_code(input[i])) {
                size_t j;

                /* Scan greedily. */
                for (j = i; j < strlen(input); j++) {
                    /* Go one step over last valid position. */
                    if (!_is_common_code(input[j]))
                        break;
                }

                /* -1: go one step back to last valid position.
                   +1: counting issue.
                 */
                size_t len = j - 1 - i + 1;

                if (len < 1) {
                    continue;
                }

                char *code = string_allocate_substring(input, i, j - 1);
                if (!code)
                    return FALSE;

                #if DEBUG
                    PUTERR("Code as token: -->%s<--", code);
                #endif

                coru_token_t *token = \
                    coru_token_new(CORU_TOKEN_CODE, code);
                if (!token) {
                    free(code);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;

                i = j - 1;
            }
            else {
                #if DEBUG
                    PUTERR("Left char: -->%c<--", input[i]);
                #endif
                /* Pass. */
            }
        }
    }

    return TRUE;
}

static BOOL _coru_lexer_expand(coru_lexer_t *self);

static BOOL _coru_lexer_push(coru_lexer_t *self, coru_token_t *token)
{
    assert(self);
    assert(token);

    if (!_coru_lexer_expand(self))
        return FALSE;

    self->tokens[self->size] = token;
    self->size += 1;

    return TRUE;
}

static BOOL _coru_lexer_expand(coru_lexer_t *self)
{
    if (self->size + 1 <= self->capacity)
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

    {
        size_t i = 0;
        while (i < self->size) {
            new_tokens[i] = old_tokens[i];
            i++;
        }
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
    return (' ' != c)   /* Space */
        && ('\t' != c)  /* TAB */
        && ('\\' != c)  /* Backslash */
        && ('\'' != c)  /* Single quote */
        && ('"' != c);  /* Double quote */
}

coru_token_t * coru_lexer_next(coru_lexer_t *self)
{
    assert(self);

    if (self->index >= self->size)
        return NULL;

    coru_token_t *token = self->tokens[self->index];
    if (!token)
        return NULL;

    self->index += 1;

    coru_token_t *copied = coru_token_copy(token);
    if (!copied)
        return NULL;

    return copied;
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
            if (tokens && tokens[i])
                coru_token_delete(tokens[i]);
        }
    }

    free(tokens);
    free(self);
}
