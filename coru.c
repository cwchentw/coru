#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if _WIN32
    #include "Shlwapi.h"
#elif __unix__ || __APPLE__
    #include <unistd.h>
    #include <sys/stat.h>
#else
    #error "Unsupported platform"
#endif

#include "coru.h"
#include "coru_argument.h"
#include "coru_command.h"
#include "coru_help.h"
#include "coru_metadata.h"
#include "coru_stats.h"
#include "hash_table.h"
#include "language.h"
#include "utils.h"

static BOOL coru_run_load(coru_argument_t * arg, char **out);

BOOL coru_run(int argc, char **argv, char **out)
{
    coru_argument_t *arg = coru_argument_parse(argc, argv);
    if (!arg)
        goto ERROR;

    if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_VERSION)) {
        coru_help_version();
    }
    else if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_LICENSE)) {
        coru_help_license();
    }
    else if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_HELP)) {
        coru_help_help(stdout);
    }
    else if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_TOO_FEW)) {
        PUTERR("No input file");
        goto ERROR;
    }
    else if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_LOAD)) {
        if (!coru_run_load(arg, out)) {
            PUTERR("Failed to load target file");
            goto ERROR;
        }
    }
    else if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_TOO_MANY)) {
        PUTERR("%s only accepts single file", CORU_PROGRAM);
        goto ERROR;
    }
    else {
        PUTERR("Unknown option");
        coru_help_help(stderr);
        goto ERROR;
    }

    coru_argument_delete(arg);

    return TRUE;

ERROR:
    if (arg)
        coru_argument_delete(arg);

    return FALSE;
}

static BOOL coru_run_load(coru_argument_t * arg, char **out)
{
    coru_stats_t *stats = NULL;
    FILE *fp = NULL;

#if _WIN32
    if (!PathFileExists(coru_argument_path(arg))) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }
#elif __unix__ || __APPLE__
    struct stat st;

    if (stat(coru_argument_path(arg), &st) & F_OK) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }
#else
    #error "Unsupported platform"
#endif

    language_t lang = LANGUAGE_UNKNOWN;

    if (LANGUAGE_UNKNOWN != coru_argument_language(arg)) {
        lang =  coru_argument_language(arg);
    }
    else {
        /* detect_target_language() may detect target language by
           opening target source. Hence, we put the statement before
           fopen() statement. */
        lang = detect_target_language(coru_argument_path(arg));
    }

#if DEBUG
    if (is_language_equal(lang, LANGUAGE_UNKNOWN)) {
        PUTERR("Unsupported language");
    } else {
        PUTS("Target language: %s", language_to_string(lang));
    }
#endif

    fp = fopen(coru_argument_path(arg), "r");
    if (!fp) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }

    stats = coru_stats_load(fp);
    if (!stats)
        goto ERROR_LOAD;

    fclose(fp);

    fp = NULL;

#if DEBUG
    PUTS("Source width: %lu", coru_stats_width(stats));
    PUTS("Source height: %lu", coru_stats_height(stats));
    /* Add stats for non-comment lines. */
#endif

    fp = fopen(coru_argument_path(arg), "r");
    if (!fp) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }

    BOOL is_all = coru_argument_is_all(arg);

    if (is_all) {
        if (!coru_load_all(fp, stats, lang, out)) {
            goto ERROR_LOAD;
        }
    }
    else {
        if (!coru_load_non_empty(fp, stats, lang, out)) {
            goto ERROR_LOAD;
        }
    }

    /* Free system resources. */
    fclose(fp);
    coru_stats_delete((void *) stats);

    return TRUE;

ERROR_LOAD:
    if (fp)
        fclose(fp);

    if (stats)
        coru_stats_delete((void *) stats);

    return FALSE;
}

static BOOL _coru_load(FILE *stream, coru_stats_t *stats, language_t lang, BOOL is_all, char **out);

BOOL coru_load_all(FILE *stream, coru_stats_t *stats, language_t lang, char **out)
{
    return _coru_load(stream, stats, lang, TRUE, out);
}

BOOL coru_load_non_empty(FILE *stream, coru_stats_t *stats, language_t lang, char **out)
{
    return _coru_load(stream, stats, lang, FALSE, out);
}

static hash_table_t * _init_comment_single_start(void);
static hash_table_t * _init_comment_single_end(void);
static hash_table_t * _init_comment_multiple_start(void);
static hash_table_t * _init_comment_multiple_end(void);

static BOOL _coru_load(FILE *stream, coru_stats_t *stats, language_t lang, BOOL is_all, char **out)
{
    char *line = NULL;
    char *lang_string = language_to_string(lang);

    hash_table_t *comment_single_start = NULL;
    hash_table_t *comment_single_end = NULL;
    hash_table_t *comment_multiple_start = NULL;
    hash_table_t *comment_multiple_end = NULL;

    comment_single_start = _init_comment_single_start();
    if (!comment_single_start)
        goto ERROR;

    comment_single_end = _init_comment_single_end();
    if (!comment_single_end)
        goto ERROR;

    comment_multiple_start = _init_comment_multiple_start();
    if (!comment_multiple_start)
        goto ERROR;

    comment_multiple_end = _init_comment_multiple_end();
    if (!comment_multiple_end)
        goto ERROR;

    char *single_start = hash_table_get(comment_single_start, lang_string);
    char *single_end = hash_table_get(comment_single_end, lang_string);
    char *multi_start = hash_table_get(comment_multiple_start, lang_string);
    char *multi_end = hash_table_get(comment_multiple_end, lang_string);

    hash_table_delete(comment_multiple_end);
    hash_table_delete(comment_multiple_start);
    hash_table_delete(comment_single_end);
    hash_table_delete(comment_single_start);

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
        + 1;  /* Trailing zero. */

#if DEBUG
    PUTS("Destination width: %lu", width_new);
#endif

    *out = (char *) malloc(coru_stats_height(stats) * width_new * sizeof(char));
    if (!out) {
        PUTERR("Failed to allocate memory for output");
        PUTERR("Check available system memory");
        goto ERROR;
    }

    (*out)[0] = '\0';  /* Strip down the string to zero. */

    size_t line_size = 150;  /* Sensible line width. */
    line = (char *) malloc(line_size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate line object");
        goto ERROR;
    }

    size_t line_number = 0;
    size_t digit_line_number;
    size_t multi = 0;

    while (fgets(line, line_size, stream)) {
        size_t sz_space;
        size_t sz_start;
        size_t sz_end;
        if (line_size == strlen(line)) {
            if ('\n' != line[line_size - 1]) {
                line_size <<= 1;
                if (!realloc(line, line_size)) {
                    PUTERR("Failed to realloc line buffer object");
                    PUTERR("Check available system memory");
                    goto ERROR;
                }
            }
            else {
                goto RELOAD_LINE;
            }
        }
        else {
RELOAD_LINE:
            /* Strip EOL */
            line[strcspn(line, "\r\n")] = 0;

            BOOL mstart = FALSE;
            BOOL mend= FALSE;

            if (strcmp("", multi_start) && string_contains(line, multi_start)) {
                multi += 1;
                mstart = TRUE;
            }

            if (strcmp("", multi_end) && string_contains(line, multi_end)) {
                multi -= 1;
                mend = TRUE;
            }

            /* Copy original text. */
            strncat(*out, line, strlen(line) + 1);

            if (multi > 0 || (mstart ^ mend)) {
                strncat(*out, END_OF_LINE, strlen(END_OF_LINE) + 1);
                continue;
            }

            if (!is_all && string_is_space_only(line)) {
                strncat(*out, END_OF_LINE, strlen(END_OF_LINE) + 1);
                continue;
            }

            line_number += 1;

            /* Insert spaces. */
            sz_space = width_new - strlen(line) - width_number
                - strlen(END_OF_LINE) - 1 /* Trailing zero */;

            /* Fix TAB issue. */
            {
                size_t i;
                for (i = 0; i < strlen(line); i++) {
                    if ('\t' == line[i])
                        sz_space -= 7;
                }
            }

            {
                size_t i;
                for (i = 0; i < sz_space; i++) {
                    strncat(*out, " ", 1 + 1);
                }
            }

            /* Insert indent. */
            strncat(*out, "  ", 2 + 1);

            /* Insert the start word of comment. */
            sz_start = strlen(single_start);
            strncat(*out, single_start, sz_start + 1);

            /* Insert a space. */
            strncat(*out, " ", 1 + 1);

            temp = line_number;
            digit_line_number = 1;
            while (temp >= 10) {
                temp /= 10;
                digit_line_number += 1;
            }

            /* Insert spaces before the line number. */
            {
                size_t i;
                for (i = 0; i < digit - digit_line_number; i++) {
                    strncat(*out, " ", 1 + 1);
                }
            }

            char *num_s = (char *) malloc(digit_line_number * sizeof(char));
            if (!num_s) {
                PUTERR("Failed to allocate memory for number string");
                PUTERR("Check available system memory");
                goto ERROR;
            }

            if (sprintf(num_s, "%lu", line_number) < 0) {
                PUTERR("Failed to insert a number");
                goto ERROR;
            }

            strncat(*out, num_s, strlen(num_s) + 1);

            free(num_s);

            if (0 != strcmp("", single_end)) {
                /* Insert a space. */
                strncat(*out, " ", 1 + 1);

                /* Insert the end word of single line comment. */
                sz_end = strlen(single_end);
                strncat(*out, single_end, sz_end + 1);
            }

            /* Insert EOL. */
            strncat(*out, END_OF_LINE, strlen(END_OF_LINE) + 1);
        }
    }

    free(line);

    return TRUE;

ERROR:
    if (line)
        free(line);

    if (comment_multiple_end)
        hash_table_delete(comment_multiple_end);

    if (comment_multiple_start)
        hash_table_delete(comment_multiple_start);

    if (comment_single_end)
        hash_table_delete(comment_single_end);

    if (comment_single_start)
        hash_table_delete(comment_single_start);

    return FALSE;
}

static hash_table_t * _init_comment_single_start(void)
{
    hash_table_t *table = hash_table_new();
    if (!table)
        return table;

    char *keys[] = {
        STRING_C,
        STRING_CPP,
        STRING_JAVA,
        STRING_CSHARP,
        STRING_PERL,
        STRING_PYTHON,
        STRING_RUBY,
        STRING_PHP,
        STRING_JS,
        STRING_GO,
        STRING_RUST,
        STRING_OBJC,
        STRING_OBJCPP,
        STRING_SWIFT,
        STRING_CSH,
        STRING_SH
    };

    char *values[] = {
        "/*",  /* C */
        "//",  /* C++ */
        "//",  /* Java */
        "//",  /* C# */
        "#",   /* Perl */
        "#",   /* Python */
        "#",   /* Ruby */
        "#",   /* PHP */
        "//",  /* JavaScript */
        "//",  /* Golang */
        "//",  /* Rust */
        "/*",  /* ObjC */
        "/*",  /* ObjC++ */
        "//",  /* Swift */
        "#",   /* C shell */
        "#"    /* Bourne shell */
    };

    {
        size_t i;
        for (i = 0; i < sizeof(keys) / sizeof(char *); i++) {
            if (!hash_table_add(table, keys[i], values[i])) {
            #if DEBUG
                PUTERR("Failed to add key-value to the hash table");
            #endif
                goto ERROR;
            }
        }
    }

    return table;

ERROR:
    if (table)
        hash_table_delete(table);

    return NULL;
}

static hash_table_t * _init_comment_single_end(void)
{
    hash_table_t *table = hash_table_new();
    if (!table)
        return table;

    char *keys[] = {
        STRING_C,
        STRING_CPP,
        STRING_JAVA,
        STRING_CSHARP,
        STRING_PERL,
        STRING_PYTHON,
        STRING_RUBY,
        STRING_PHP,
        STRING_JS,
        STRING_GO,
        STRING_RUST,
        STRING_OBJC,
        STRING_OBJCPP,
        STRING_SWIFT,
        STRING_CSH,
        STRING_SH
    };

    char *values[] = {
        "*/",  /* C */
        "",    /* C++ */
        "",    /* Java */
        "",    /* C# */
        "",    /* Perl */
        "",    /* Python */
        "",    /* Ruby */
        "",    /* PHP */
        "",    /* JavaScript */
        "",    /* Golang */
        "",    /* Rust */
        "*/",  /* ObjC */
        "*/",  /* ObjC++ */
        "",    /* Swift */
        "",    /* C shell */
        ""     /* Bourne shell */
    };

    {
        size_t i;
        for (i = 0; i < sizeof(keys) / sizeof(char *); i++) {
            if (!hash_table_add(table, keys[i], values[i])) {
            #if DEBUG
                PUTERR("Failed to add key-value to the hash table");
            #endif
                goto ERROR;
            }
        }
    }

    return table;

ERROR:
    if (table)
        hash_table_delete(table);

    return NULL;
}

static hash_table_t * _init_comment_multiple_start(void)
{
    hash_table_t *table = hash_table_new();
    if (!table)
        return table;

    char *keys[] = {
        STRING_C,
        STRING_CPP,
        STRING_JAVA,
        STRING_CSHARP,
        STRING_PERL,
        STRING_PYTHON,
        STRING_RUBY,
        STRING_PHP,
        STRING_JS,
        STRING_GO,
        STRING_RUST,
        STRING_OBJC,
        STRING_OBJCPP,
        STRING_SWIFT,
        STRING_CSH,
        STRING_SH
    };

    char *values[] = {
        "/*",  /* C */
        "/*",  /* C++ */
        "/*",  /* Java */
        "/*",  /* C# */
        "",    /* Perl */
        "",    /* Python */
        "",    /* Ruby */
        "/*",  /* PHP */
	    "/*",  /* JavaScript */
        "/*",  /* Golang */
        "/*",  /* Rust */
        "/*",  /* ObjC */
        "/*",  /* ObjC++ */
        "/*",  /* Swift */
        "",    /* C shell */
        ""     /* Bourne shell */
    };

    {
        size_t i;
        for (i = 0; i < sizeof(keys) / sizeof(char *); i++) {
            if (!hash_table_add(table, keys[i], values[i])) {
            #if DEBUG
                PUTERR("Failed to add key-value to the hash table");
            #endif
                goto ERROR;
            }
        }
    }

    return table;

ERROR:
    if (table)
        hash_table_delete(table);

    return NULL;
}

static hash_table_t * _init_comment_multiple_end(void)
{
    hash_table_t *table = hash_table_new();
    if (!table)
        return table;

    char *keys[] = {
        STRING_C,
        STRING_CPP,
        STRING_JAVA,
        STRING_CSHARP,
        STRING_PERL,
        STRING_PYTHON,
        STRING_RUBY,
        STRING_PHP,
        STRING_JS,
        STRING_GO,
        STRING_RUST,
        STRING_OBJC,
        STRING_OBJCPP,
        STRING_SWIFT,
        STRING_CSH,
        STRING_SH
    };

    char *values[] = {
        "*/",  /* C */
        "*/",  /* C++ */
        "*/",  /* Java */
        "*/",  /* C# */
        "",    /* Perl */
        "",    /* Python */
        "",    /* Ruby */
        "*/",  /* PHP */
        "*/",  /* JavaScript */
        "*/",  /* Golang */
        "*/",  /* Rust */
        "*/",  /* ObjC */
        "*/",  /* ObjC++ */
        "*/",  /* Swift */
        "",    /* C shell */
        ""     /* Bourne shell */
    };

    {
        size_t i;
        for (i = 0; i < sizeof(keys) / sizeof(char *); i++) {
            if (!hash_table_add(table, keys[i], values[i])) {
            #if DEBUG
                PUTERR("Failed to add key-value to the hash table");
            #endif
                goto ERROR;
            }
        }
    }

    return table;

ERROR:
    if (table)
        hash_table_delete(table);

    return NULL;
}
