#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "coru.h"
#include "coru_lexer.h"
#include "coru_token.h"
#include "cstring.h"
#include "print.h"

/* Predefined characters in coru lexer. */
#define SPACE         ' '
#define TAB           '\t'
#define BACKSLASH     '\\'
#define SINGLE_QUOTE  '\''
#define DOUBLE_QUOTE  '"'
#define AMPERSAND     '&'
#define BACKTICK      '`'

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

#define IS_CODE(c) \
    ((SPACE != (c)) \
      && (TAB != (c)) \
      && (BACKSLASH != (c)) \
      && (SINGLE_QUOTE != (c)) \
      && (DOUBLE_QUOTE != (c)) \
      && (AMPERSAND != (c)) \
      && (BACKTICK != (c)))

/* Trade-off: define "code" as a complement set of special characters.
 * Simple and fast, but tightly couples tokenization to known delimiters. */


static BOOL _coru_lexer_push(coru_lexer_t *self, coru_token_t *token);

BOOL coru_lexer_lex(coru_lexer_t *self, char *input)
{
    assert(self);

#if DEBUG
    PUTERR("Source to scan: -->%s<--", input);
#endif
    {
        size_t i;
        /* Trade-off: hand-written lexer instead of formal lexer generator.
         * Easier to control for small DSL, but more imperative and less declarative. */
        for (i = 0; i < strlen(input); i++) {
            if (SPACE == input[i]) {
                size_t j;

                /* Intent: group consecutive spaces into a single token */
                /* Trade-off: greedy scanning reduces token count, but loses fine-grained structure */
                for (j = i; j < strlen(input); j++) {
                    /* Go one step over last valid position. */
                    if (SPACE != input[j])
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
            else if (TAB == input[i]) {
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
            else if (SINGLE_QUOTE == input[i]) {
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
            else if (DOUBLE_QUOTE == input[i]) {
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
            else if (BACKSLASH == input[i]) {
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
            else if (AMPERSAND == input[i]) {
                char *ampersand = string_allocate("&");
                if (!ampersand)
                    return FALSE;
            #if DEBUG
                PUTERR("Ampersand as token: -->%s<--", ampersand);
            #endif

                coru_token_t *token = \
                    coru_token_new(CORU_TOKEN_AMPERSAND, ampersand);
                if (!token) {
                    free(ampersand);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;
            }
            else if (BACKTICK == input[i]) {
                char *backtick = string_allocate("`");
                if (!backtick)
                    return FALSE;
            #if DEBUG
                PUTERR("Backtick as token: -->%s<--", backtick);
            #endif

                coru_token_t *token = \
                    coru_token_new(CORU_TOKEN_BACKTICK, backtick);
                if (!token) {
                    free(backtick);
                    return FALSE;
                }

                if (!_coru_lexer_push(self, token))
                    return FALSE;
            }
            else if (IS_CODE(input[i])) {
                size_t j;

                /* Intent: treat continuous non-special characters as a code fragment */
                /* Trade-off: no further parsing inside code tokens at this stage */
                for (j = i; j < strlen(input); j++) {
                    /* Go one step over last valid position. */
                    if (!IS_CODE(input[j]))
                        break;
                }

                /* -1: go one step back to last valid position.
                   +1: counting issue.
                 */
                size_t len = j - 1 - i + 1;

                if (len < 1)
                    continue;

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
                /* Trade-off: unknown characters are ignored instead of producing errors.
                 * Keeps lexer permissive, but may hide malformed input. */
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

    /* Trade-off: store tokens in a dynamic array (simple buffer model)
     * instead of streaming or iterator-based lexing. */
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
            /* Copy the address of the token. */
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

coru_token_t * coru_lexer_next(coru_lexer_t *self)
{
    assert(self);

    if (self->index >= self->size)
        return NULL;

    coru_token_t *token = self->tokens[self->index];
    if (!token)
        return NULL;

    self->index += 1;

    /* Trade-off: return a copy of token to isolate ownership.
     * Safer, but adds allocation overhead. */
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
