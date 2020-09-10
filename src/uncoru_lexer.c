#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "cstring.h"
#include "print.h"
#include "uncoru_lexer.h"
#include "uncoru_token.h"

struct uncoru_lexer_t {
    size_t size;
    size_t capacity;
    size_t index;
    uncoru_token_t **tokens;
    char *comment_start;
    char *comment_end;
};

uncoru_lexer_t * uncoru_lexer_new(void)
{
    uncoru_lexer_t *lexer = \
        (uncoru_lexer_t *) malloc(sizeof(uncoru_lexer_t));
    if (!lexer) {
        PUTERR("Failed to allocate memory for uncoru lexer object");
        PUTERR("Check available system memory");
        return lexer;
    }

    lexer->size = 0;
    lexer->capacity = 16;
    lexer->index = 0;

    lexer->tokens = \
        (uncoru_token_t **) malloc(lexer->capacity * sizeof(uncoru_token_t *));
    if (!(lexer->tokens)) {
        PUTERR("Failed to allocate the tokens of uncoru lexer");
        PUTERR("Check available system memory");
        free(lexer);
        return NULL;
    }

    {
        size_t i;
        for (i = 0; i < lexer->capacity; i++)
            lexer->tokens[i] = NULL;
    }

    lexer->comment_start = NULL;
    lexer->comment_end = NULL;

    return lexer;
}

void uncoru_lexer_delete(void *self)
{
    assert(self);

    size_t capacity = ((uncoru_lexer_t *) self)->capacity;
    uncoru_token_t **tokens = ((uncoru_lexer_t *) self)->tokens;
    {
        size_t i;
        for (i = 0; i < capacity; i++) {
            if (tokens[i])
                uncoru_token_delete((void *) tokens[i]);
        }
    }

    free((void *) tokens);
    free(self);
}

void uncoru_lexer_set_comment_start(uncoru_lexer_t *self, char *comment)
{
    assert(self);

    self->comment_start = comment;
}

void uncoru_lexer_set_comment_end(uncoru_lexer_t *self, char *comment)
{
    assert(self);

    self->comment_end = comment;
}

/* Predefined characters in uncoru lexer. */
#define SPACE         ' '
#define TAB           '\t'
#define BACKSLASH     '\\'
#define SINGLE_QUOTE  '\''
#define DOUBLE_QUOTE  '"'

#define IS_COMMENT_START(c) \
    (self->comment_start \
     && *(self->comment_start) == (c))

#define IS_COMMENT_END(c) \
    (self->comment_end \
     && 0 != strcmp("", self->comment_end) \
     && *(self->comment_end) == (c))

#define IS_CODE(c) \
    (SPACE != (c) \
     && TAB != (c) \
     && !isdigit(c) \
     && BACKSLASH != (c) \
     && SINGLE_QUOTE != (c) \
     && DOUBLE_QUOTE != (c))

static BOOL _uncoru_lexer_push(uncoru_lexer_t *self, uncoru_token_t *token);

BOOL uncoru_lexer_lex(uncoru_lexer_t *self, char *input)
{
    assert(self);

#if DEBUG
    PUTERR("Source to scan: -->%s<--", input);
#endif
    {
        size_t i;
        for (i = 0; i < strlen(input); i++) {
            if (SPACE == input[i]) {
                size_t j;

                /* Scan greedily. */
                for (j = i; j < strlen(input); j++) {
                    if (SPACE != input[j])
                        break;
                }

                size_t len = j - 1 + i + 1;

                if (len < 1)
                    continue;

                char *spaces = string_allocate_substring(input, i, j - 1);
                if (!spaces)
                    return FALSE;

            #if DEBUG
                PUTERR("Space as token: -->%s<--", spaces);
            #endif

                uncoru_token_t *token = \
                    uncoru_token_new(UNCORU_TOKEN_SPACE, spaces);
                if (!token) {
                    free(spaces);
                    return FALSE;
                }

                if (!_uncoru_lexer_push(self, token))
                    return FALSE;

                i = j - 1;
            }
            else if (TAB == input[i]) {
                char *tab = string_allocate("\t");
                if (!tab)
                    return FALSE;

            #if DEBUG
                PUTERR("TAB as token: -->%s<--", tab);
            #endif

                uncoru_token_t *token = uncoru_token_new(UNCORU_TOKEN_TAB, tab);
                if (!token) {
                    free(tab);
                    return FALSE;
                }

                if (!_uncoru_lexer_push(self, token))
                    return FALSE;
            }
            else if (SINGLE_QUOTE == input[i]) {
                char *quote = string_allocate("'");
                if (!quote)
                    return FALSE;

            #if DEBUG
                PUTERR("Single quote as token: -->%s<--", quote);
            #endif

                uncoru_token_t *token = \
                    uncoru_token_new(UNCORU_TOKEN_SINGLE_QUOTE, quote);
                if (!token) {
                    free(quote);
                    return FALSE;
                }

                if (!_uncoru_lexer_push(self, token))
                    return FALSE;
            }
            else if (DOUBLE_QUOTE == input[i]) {
                char *quote = string_allocate("\"");
                if (!quote)
                    return FALSE;

            #if DEBUG
                PUTERR("Double quote as token: -->%s<--", quote);
            #endif

                uncoru_token_t *token = \
                    uncoru_token_new(UNCORU_TOKEN_DOUBLE_QUOTE, quote);
                if (!token) {
                    free(quote);
                    return FALSE;
                }

                if (!_uncoru_lexer_push(self, token))
                    return FALSE;
            }
            else if (BACKSLASH == input[i]) {
                char *backslash = string_allocate("\\");
                if (!backslash)
                    return FALSE;
            #if DEBUG
                PUTERR("Backslash as token: -->%s<--", backslash);
            #endif

                uncoru_token_t *token = \
                    uncoru_token_new(UNCORU_TOKEN_BACKSLASH, backslash);
                if (!token) {
                    free(backslash);
                    return FALSE;
                }

                if (!_uncoru_lexer_push(self, token))
                    return FALSE;
            }
            else if (isdigit(input[i])) {
                size_t j;

                for (j = i; j < strlen(input); j++) {
                    if (!isdigit(input[j]))
                        break;
                }

                size_t len = j - i;

                if (len < 1)
                    continue;

                char *integer = string_allocate_substring(input, i, j - 1);
                if (!integer)
                    return FALSE;

            #if DEBUG
                PUTERR("Integer as token: -->%s<--", integer);
            #endif

                uncoru_token_t *token = \
                    uncoru_token_new(UNCORU_TOKEN_INTEGER, integer);
                if (!token) {
                    free(integer);
                    return FALSE;
                }

                if (!_uncoru_lexer_push(self, token))
                    return FALSE;

                i = j - 1;
            }
            else if (IS_COMMENT_START(input[i])) {
                size_t j;

                for (j = i;
                     j < strlen(input) && j - i < strlen(self->comment_start);
                     j++) {
                    if (self->comment_start[j-i] != input[j])
                        break;
                }

                if (j - i < strlen(self->comment_start))
                    goto SCAN_CODE;

                char *comment = string_allocate(self->comment_start);
                if (!comment)
                    return FALSE;

            #if DEBUG
                PUTERR("Comment start as token: -->%s<--", comment);
            #endif

                uncoru_token_t *token = \
                    uncoru_token_new(UNCORU_TOKEN_COMMENT_START, comment);
                if (!token)
                    return FALSE;

                if (!_uncoru_lexer_push(self, token))
                    return FALSE;

                i = j - 1;
            }
            else if (IS_COMMENT_END(input[i])) {
                size_t j;

                for (j = i;
                     j < strlen(input) && j - i < strlen(self->comment_end);
                     j++) {
                    if (self->comment_end[j-i] != input[j])
                        break;
                }

                if (j - i < strlen(self->comment_end))
                    goto SCAN_CODE;

                char *comment = string_allocate(self->comment_end);
                if (!comment)
                    return FALSE;

            #if DEBUG
                PUTERR("Comment end as token: -->%s<--", comment);
            #endif

                uncoru_token_t *token = \
                    uncoru_token_new(UNCORU_TOKEN_COMMENT_END, comment);
                if (!token)
                    return FALSE;

                if (!_uncoru_lexer_push(self, token))
                    return FALSE;

                i = j - 1;
            }
            else if (IS_CODE(input[i])) {
            SCAN_CODE:
                1;  /* Trick for label. */
                size_t j;

                for (j = i; j < strlen(input); j++) {
                    if (!IS_CODE(input[j]))
                        break;
                }

                size_t len = j - i;

                if (len < 1)
                    continue;

                char *code = string_allocate_substring(input, i, j - 1);
                if (!code)
                    return FALSE;

            #if DEBUG
                PUTERR("Code as token: -->%s<--", code);
            #endif

                uncoru_token_t *token = \
                    uncoru_token_new(UNCORU_TOKEN_CODE, code);
                if (!token) {
                    free(code);
                    return FALSE;
                }

                if (!_uncoru_lexer_push(self, token))
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

static BOOL _uncoru_lexer_expand(uncoru_lexer_t *self);

static BOOL _uncoru_lexer_push(uncoru_lexer_t *self, uncoru_token_t *token)
{
    assert(self);
    assert(token);

    if (!_uncoru_lexer_expand(self))
        return FALSE;

    self->tokens[self->size] = token;
    self->size += 1;

    return TRUE;
}

static BOOL _uncoru_lexer_expand(uncoru_lexer_t *self)
{
    assert(self);

    if (self->size + 1 <= self->capacity)
        return TRUE;

    self->capacity <<= 1;

    uncoru_token_t **old_tokens = self->tokens;
    uncoru_token_t **new_tokens = \
        (uncoru_token_t **) malloc(self->capacity * sizeof(uncoru_token_t *));
    if (!new_tokens) {
        PUTERR("Failed to allocate new tokens of uncoru lexer");
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

uncoru_token_t * coru_lexer_next(uncoru_lexer_t *self)
{
    assert(self);

    if (self->index >= self->size)
        return NULL;

    uncoru_token_t *token = self->tokens[self->index];
    if (!token)
        return NULL;

    self->index += 1;

    uncoru_token_t *copied = uncoru_token_copy(token);
    if (!copied)
        return NULL;

    return copied;
}

uncoru_token_t * coru_lexer_peek_n(uncoru_lexer_t *self, size_t n)
{
    assert(self);

    if (self->index + n >= self->size)
        return NULL;

    uncoru_token_t *token = self->tokens[self->index + n];
    if (!token)
        return NULL;

    return token;
}
