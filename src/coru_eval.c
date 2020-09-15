#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coru.h"
#include "coru_eval.h"
#include "coru_lexer.h"
#include "coru_parser.h"
#include "coru_stats.h"
#include "cstring.h"
#include "hash_table.h"
#include "language.h"
#include "print.h"
#include "syntax_data.h"

struct coru_eval_t {
    BOOL first_line;
    BOOL mstart;
    BOOL mend;
    size_t multi;
    size_t line_number;
};

coru_eval_t * coru_eval_new(void)
{
    coru_eval_t *eval = \
        (coru_eval_t *) malloc(sizeof(coru_eval_t));
    if (!eval) {
        PUTERR("Failed to allocate memory for coru eval object");
        PUTERR("Check available system memory");
        return eval;
    }

    eval->first_line = TRUE;
    eval->mstart = FALSE;
    eval->mend = FALSE;
    eval->multi = 0;
    eval->line_number = 0;

    return eval;
}

extern hash_table_t *comment_single_start;
extern hash_table_t *comment_single_end;
extern hash_table_t *comment_multiple_start;
extern hash_table_t *comment_multiple_end;

BOOL coru_eval_eval(coru_eval_t *self,
    coru_stats_t *stats,
    language_t lang,
    BOOL is_all,
    char *line, char **out)
{
    assert(self);

    char *single_start = NULL;
    char *single_end = NULL;
    char *multi_start = NULL;
    char *multi_end = NULL;

    coru_lexer_t *lexer = NULL;
    coru_parser_t *parser = NULL;

    if (!comment_single_start) {
        comment_single_start = init_comment_single_start();
        if (!comment_single_start)
            goto ERROR_CORU_EVAL;
    }

    if (!comment_single_end) {
        comment_single_end = init_comment_single_end();
        if (!comment_single_end)
            goto ERROR_CORU_EVAL;
    }

    if (!comment_multiple_start) {
        comment_multiple_start = init_comment_multiple_start();
        if (!comment_multiple_start)
            goto ERROR_CORU_EVAL;
    }

    if (!comment_multiple_end) {
        comment_multiple_end = init_comment_multiple_end();
        if (!comment_multiple_end)
            goto ERROR_CORU_EVAL;
    }

    char *lang_string = language_to_string(lang);

    single_start = hash_table_get(comment_single_start, lang_string);
    if (!single_start)
        goto ERROR_CORU_EVAL;

    single_end = hash_table_get(comment_single_end, lang_string);
    if (!single_end)
        goto ERROR_CORU_EVAL;

    multi_start = hash_table_get(comment_multiple_start, lang_string);
    if (!multi_start)
        goto ERROR_CORU_EVAL;

    multi_end = hash_table_get(comment_multiple_end, lang_string);
    if (!multi_end)
        goto ERROR_CORU_EVAL;

    /* The format of line number:
       *start*    1 *end*
     ^^ --> indent after original source code
       ^^^^^^^ --> start word of comment
              ^ --> one space
               ^^^ --> indent for line number
                  ^ --> line number
                   ^ --> one space (optional)
                    ^^^^^ --> end word of comment (optional)
     */
    /* Add line numbers to source later. */
    size_t indent = 2;
    size_t space = 1;

    size_t digit = 1;
    size_t temp = coru_stats_height(stats);
    while (temp >= 10) {
        temp /= 10;
        digit += 1;
    }

    size_t width_number = indent + strlen(single_start) + space + digit;

    if (0 != strcmp("", single_end))
        width_number += space + strlen(single_end);

    size_t width_new = coru_stats_width(stats) + width_number \
        + strlen(END_OF_LINE);

    #if DEBUG
        PUTS("Destination width: %lu", width_new);
    #endif

    if (!(*out)) {
        *out = (char *) malloc(
            (coru_stats_height(stats) * width_new + 1) * sizeof(char));
        if (!(*out)) {
            PUTERR("Failed to allocate memory for output");
            PUTERR("Check available system memory");
            goto ERROR_CORU_EVAL;
        }

        (*out)[0] = '\0';  /* Strip down the string to zero. */
    }

    size_t total_size = 0 + strlen(*out);

    /* Detect #! (shebang) on first line. */
    if (self->first_line) {
        if (string_starts_with(line, "#!")) {
            size_t sz = strlen(line);
            strcpy((*out)+total_size, line);
            total_size += sz;
            (*out)[total_size] = '\0';

            self->first_line = FALSE;
            return TRUE;
        }

        self->first_line = FALSE;
    }

    /* FIXME: We remove a trailing newline.
         But it wrongly consume the last character. */
    {
        size_t len = strlen(line);

        line[len-1] = '\0';
    }

    self->mstart = FALSE;
    self->mend = FALSE;

    if (strcmp("", multi_start) && string_contains(line, multi_start)) {
        self->multi += 1;
        self->mstart = TRUE;
    }

    if (strcmp("", multi_end) && string_contains(line, multi_end)) {
        self->multi -= 1;
        self->mend = TRUE;
    }

    /* Copy original text. */
    {
        size_t sz = strlen(line);
        strcpy((*out)+total_size, line);
        total_size += sz;
        (*out)[total_size] = '\0';
    }

    if (self->multi > 0 || (self->mstart ^ self->mend)) {
        size_t sz = strlen(END_OF_LINE);
        strcpy((*out)+total_size, END_OF_LINE);
        total_size += sz;
        (*out)[total_size] = '\0';

        return TRUE;
    }

    if (!is_all && string_is_space_only(line)) {
        size_t sz = strlen(END_OF_LINE);
        strcpy((*out)+total_size, END_OF_LINE);
        total_size += sz;
        (*out)[total_size] = '\0';

        return TRUE;
    }

    lexer = coru_lexer_new();
    if (!lexer)
        goto ERROR_CORU_EVAL;

    if (!coru_lexer_lex(lexer, line)) {
        PUTERR("Failed to lex input");
        goto ERROR_CORU_EVAL;
    }

    parser = coru_parser_new();
    if (!parser)
        goto ERROR_CORU_EVAL;

    if (!coru_parser_parse(parser, lexer)) {
        PUTERR("Failed to parse input");
        goto ERROR_CORU_EVAL;
    }

    /* Insert spaces. */
    size_t sz_space = width_new - strlen(line) - width_number
        - strlen(END_OF_LINE) - 1 /* Trailing zero */;

    coru_ast_t *ast = coru_parser_next(parser);
    while (ast) {
        if (CORU_AST_BACKSLASH == coru_ast_type(ast)
            && LANGUAGE_FORTRAN != lang) {
            size_t sz = strlen(END_OF_LINE);
            strcpy((*out)+total_size, END_OF_LINE);
            total_size += sz;
            (*out)[total_size] = '\0';

            goto END_CORU_EVAL;
        }
        else if (CORU_AST_AMPERSAND == coru_ast_type(ast)
                 && LANGUAGE_FORTRAN == lang) {
            size_t sz = strlen(END_OF_LINE);
            strcpy((*out)+total_size, END_OF_LINE);
            total_size += sz;
            (*out)[total_size] = '\0';

            goto END_CORU_EVAL;
        }
        else if (CORU_AST_TAB == coru_ast_type(ast)) {
            sz_space -= 7;
        }

        ast = coru_parser_next(parser);
    }

    {
        size_t i;
        for (i = 0; i < sz_space; i++) {
            size_t sz = strlen(" ");
            strcpy((*out)+total_size, " ");
            total_size += sz;
            (*out)[total_size] = '\0';
        }
    }

    self->line_number += 1;

    /* Insert indent. */
    {
        size_t sz = strlen("  ");
        strcpy((*out)+total_size, "  ");
        total_size += sz;
        (*out)[total_size] = '\0';
    }


    /* Insert the start word of comment. */
    {
        size_t sz = strlen(single_start);
        strcpy((*out)+total_size, single_start);
        total_size += sz;
        (*out)[total_size] = '\0';
    }

    /* Insert a space. */
    {
        size_t sz = strlen(" ");
        strcpy((*out)+total_size, " ");
        total_size += sz;
        (*out)[total_size] = '\0';
    }


    temp = self->line_number;
    size_t digit_line_number = 1;
    while (temp >= 10) {
        temp /= 10;
        digit_line_number += 1;
    }

    /* Insert spaces before the line number. */
    {
        size_t i;
        for (i = 0; i < digit - digit_line_number; i++) {
            size_t sz = strlen(" ");
            strcpy((*out)+total_size, " ");
            total_size += sz;
            (*out)[total_size] = '\0';
        }
    }

    char *num_s = (char *) malloc((digit_line_number + 1) * sizeof(char));
    if (!num_s) {
        PUTERR("Failed to allocate memory for number string");
        PUTERR("Check available system memory");
        goto ERROR_CORU_EVAL;
    }

    num_s[0] = '\0';

    if (sprintf(num_s, "%lu", self->line_number) < 0) {
        PUTERR("Failed to insert a number");
        goto ERROR_CORU_EVAL;
    }

    {
        size_t sz = strlen(num_s);
        strcpy((*out)+total_size, num_s);
        total_size += sz;
        (*out)[total_size] = '\0';
    }

    free(num_s);

    if (0 != strcmp("", single_end)) {
        /* Insert a space. */
        {
            size_t sz = strlen(" ");
            strcpy((*out)+total_size, " ");
            total_size += sz;
            (*out)[total_size] = '\0';
        }

        /* Insert the end word of single line comment. */
        {
            size_t sz = strlen(single_end);
            strcpy((*out)+total_size, single_end);
            total_size += sz;
            (*out)[total_size] = '\0';
        }
    }

    /* Insert EOL. */
    {
        size_t sz = strlen(END_OF_LINE);
        strcpy((*out)+total_size, END_OF_LINE);
        total_size += sz;
        (*out)[total_size] = '\0';
    }

END_CORU_EVAL:
    coru_parser_delete(parser);
    coru_lexer_delete(lexer);

    return TRUE;

ERROR_CORU_EVAL:
    if (parser)
        coru_parser_delete(parser);

    if (lexer)
        coru_lexer_delete(lexer);

    return FALSE;
}

void coru_eval_delete(void *self)
{
    if (!self)
        return;

    free(self);
}
