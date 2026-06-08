#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "coru_ast.h"
#include "coru_lexer.h"
#include "coru_parser.h"
#include "coru_stats.h"
#include "coru_token.h"
#include "print.h"

static coru_stats_t * coru_stats_new(void);
static BOOL _coru_stats_eval(coru_stats_t *self, char *line);

coru_stats_t * coru_stats_load_fs(FILE *stream)
{
    char *line = NULL;
    char *more_line = NULL;

    size_t line_size = 150;  /* Sensible line width */
    line = (char *) malloc(line_size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate line object");
        PUTERR("Check available system memory");
        return NULL;
    }

    line[0] = '\0';

    coru_stats_t *stats = coru_stats_new();
    if (!stats)
        goto ERROR_CORU_STATS;

    while (fgets(line, line_size, stream)) {
        if (line_size == strlen(line)) {
            if ('\n' != line[line_size - 1]) {
                line_size <<= 1;
                more_line = realloc(line, line_size);
                if (!more_line) {
                    PUTERR("Failed to realloc line buffer object");
                    PUTERR("Check available system memory");
                    goto ERROR_CORU_STATS;
                }
                else {
                    line = more_line;
                }
            }
            else {
                goto LOAD_LINE;
            }
        }
        else {
LOAD_LINE:
            if (!_coru_stats_eval(stats, line))
                goto ERROR_CORU_STATS;
        }
    }

    free(line);

    return stats;

ERROR_CORU_STATS:
    if (line)
        free(line);

    if (stats)
        coru_stats_delete(stats);

    return NULL;
}

static coru_stats_t * coru_stats_new()
{
    coru_stats_t *stats = (coru_stats_t *) malloc(sizeof(coru_stats_t));
    if (!stats) {
        PUTERR("Failed to allocate memory for stats object");
        PUTERR("Check available system memory");
        return stats;
    }

    stats->width = 0;
    stats->height = 0;

    return stats;
}

static BOOL _coru_stats_eval(coru_stats_t *self, char *line)
{
    coru_lexer_t lexer;
    coru_parser_t parser;

    if (coru_lexer_new(&lexer))
        goto ERROR_CORU_STATS;

    if (!coru_lexer_lex(&lexer, line)) {
        PUTERR("Failed to lex input");
        goto ERROR_CORU_STATS;
    }

    if (coru_parser_new(&parser))
        goto ERROR_CORU_STATS;

    if (!coru_parser_parse(&parser, &lexer)) {
        PUTERR("Failed to parse input");
        goto ERROR_CORU_STATS;
    }

    size_t sz_line = strlen(line);

    coru_ast_t *ast = coru_parser_next(&parser);
    while (ast) {
        if (CORU_AST_TAB == coru_ast_type(ast))
            sz_line += 8;

        ast = coru_parser_next(&parser);
    }

    if (sz_line > coru_stats_width(self)) {
        coru_stats_set_width(self, sz_line);
    }

    coru_stats_set_height(self, coru_stats_height(self) + 1);

    coru_parser_delete(&parser);
    coru_lexer_delete(&lexer);

    return TRUE;

ERROR_CORU_STATS:
    coru_parser_delete(&parser);
    coru_lexer_delete(&lexer);

    return FALSE;
}
