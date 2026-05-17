#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "language.h"
#include "print.h"
#include "syntax_data.h"
#include "uncoru_ast.h"
#include "uncoru_eval.h"
#include "uncoru_lexer.h"
#include "uncoru_parser.h"
#include "uncoru_stats.h"

BOOL uncoru_eval_eval(
    uncoru_eval_t *self,
    uncoru_stats_t *stats,
    language_t lang,
    char *line, char **out)
{
    assert(self);

    char *comment_start = NULL;
    char *comment_end = NULL;
    uncoru_lexer_t lexer;
    uncoru_parser_t parser;

    char *lang_string = language_to_string(lang);

    comment_start = single_start_string(lang_string);
    if (!comment_start)
        goto ERROR_UNCORU_EVAL;

    comment_end = single_end_string(lang_string);
    if (!comment_end)
        goto ERROR_UNCORU_EVAL;

    if (!(*out)) {
        *out = (char *) malloc(
            (uncoru_stats_width(stats) * uncoru_stats_height(stats) + 1)
             * sizeof(char));
        if (!(*out))
            goto ERROR_UNCORU_EVAL;

        (*out)[0] = '\0';
    }

    if (uncoru_lexer_new(&lexer))
        goto ERROR_UNCORU_EVAL;

    uncoru_lexer_set_comment_start(&lexer, comment_start);
#if DEBUG
    PUTERR("The start comment: -->%s<--", uncoru_lexer_comment_start(lexer));
#endif

    uncoru_lexer_set_comment_end(&lexer, comment_end);
#if DEBUG
    PUTERR("The end comment: -->%s<--", uncoru_lexer_comment_end(lexer));
#endif

    if (!uncoru_lexer_lex(&lexer, line))
        goto ERROR_UNCORU_EVAL;

    if (uncoru_parser_new(&parser))
        goto ERROR_UNCORU_EVAL;

    if (!uncoru_parser_parse(&parser, &lexer)) {
        PUTERR("Failed to parse input");
        goto ERROR_UNCORU_EVAL;
    }

    size_t sz = 0 + strlen(*out);

    uncoru_ast_t *ast = uncoru_parser_peek_n(&parser, 0);
    while (ast) {
        if (UNCORU_AST_SPACE == uncoru_ast_type(ast)) {
            uncoru_ast_t *astLineNumber = uncoru_parser_peek_n(&parser, 1);

            if (!astLineNumber)
                goto PARSE_COMMON_AST;
            else if (UNCORU_AST_LINE_NUNBER != uncoru_ast_type(astLineNumber))
                goto PARSE_COMMON_AST;

            /* Discard the space ast. */
            uncoru_parser_next(&parser);

            /* Discard the line number ast. */
            uncoru_parser_next(&parser);

            goto PARSE_LINE_NUMBER_AST;
        }
        else {
        PARSE_COMMON_AST:
            /* Consume one token. */
            ast = uncoru_parser_next(&parser);

            char *txt = uncoru_ast_text(ast);

        #if DEBUG
            if (txt)
                PUTERR("Print text to output: -->%s<--", txt);
        #endif

            if (txt) {
                strcpy((*out)+sz, txt);
                sz += strlen(txt);

                free(txt);
            }
        }

    PARSE_LINE_NUMBER_AST:
        /* Discard the space before the line number. */

        ast = uncoru_parser_peek_n(&parser, 0);
    }

    (*out)[sz] = '\0';  /* Trailing zero. */

    uncoru_parser_delete(&parser);
    uncoru_lexer_delete(&lexer);

    return TRUE;

ERROR_UNCORU_EVAL:
    uncoru_parser_delete(&parser);
    uncoru_lexer_delete(&lexer);

    return FALSE;
}
