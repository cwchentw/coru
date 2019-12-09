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

#include "boolean.h"
#include "coru_argument.h"
#include "coru_command.h"
#include "coru_help.h"
#include "coru_metadata.h"
#include "coru_stats.h"
#include "hash_table.h"
#include "language.h"
#include "utils.h"

static BOOL coru_run_load(coru_argument_t * arg, char **out);

BOOL coru_run(coru_argument_t * arg, char **out)
{
    if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_VERSION)) {
        help_version();
        return TRUE;
    }
    else if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_LICENSE)) {
        help_license();
        return TRUE;
    }
    else if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_HELP)) {
        help_help(stdout);
        return TRUE;
    }
    else if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_TOO_FEW)) {
        PUTERR("No input file");
        return FALSE;
    }
    else if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_LOAD)) {
        if (!coru_run_load(arg, out)) {
            PUTERR("Failed to load target file");
            return FALSE;
        }

        return TRUE;
    }
    else if (is_coru_command_equal(coru_argument_command(arg), CORU_COMMAND_TOO_MANY)) {
        PUTERR("%s only accepts single file", CORU_PROGRAM);
        return FALSE;
    }
    else {
        PUTERR("Unknown option");
        help_help(stderr);
        return FALSE;
    }
}

static hash_table_t * _init_comment_single_start(void);
static hash_table_t * _init_comment_single_end(void);
static hash_table_t * _init_comment_multiple_start(void);
static hash_table_t * _init_comment_multiple_end(void);

static BOOL coru_run_load(coru_argument_t * arg, char **out)
{
    coru_stats_t *stats = NULL;
    FILE *fp = NULL;
    char *line = NULL;
    hash_table_t *comment_single_start = NULL;
    hash_table_t *comment_single_end = NULL;
    hash_table_t *comment_multiple_start = NULL;
    hash_table_t *comment_multiple_end = NULL;

    stats = coru_stats_new();
    if (!stats) {
    #if DEBUG
        PUTERR("Failed to load stats");
    #endif
        goto ERROR_LOAD;
    }

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

    /* Check this statement later. */
    /* detect_target_language() may detect target language by
       opening target source. Hence, we put the statement before
       fopen() statement. */
    language_t lang = detect_target_language(coru_argument_path(arg));

    fp = fopen(coru_argument_path(arg), "r");
    if (!fp) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }
#if DEBUG
    if (is_language_equal(lang, LANGUAGE_UNKNOWN)) {
        PUTERR("Unsupported language");
    } else {
        PUTS("Target language: %s", language_to_string(lang));
    }
#endif

    size_t line_size = 150;  /* Sensible line size */
    line = (char *) malloc(line_size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate line object");
        goto ERROR_LOAD;
    }

    size_t sz_line;
    while (fgets(line, line_size, fp)) {
        if (line_size == strlen(line)) {
            if ('\n' != line[line_size - 1]) {
                line_size <<= 1;
                if (!realloc(line, line_size)) {
                    PUTERR("Failed to realloc line buffer object");
                    PUTERR("Check available system memory");
                    goto ERROR_LOAD;
                }
            }
            else {
                goto LOAD_LINE;
            }
        }
        else {
LOAD_LINE:
            /* Fix TAB issue */
            sz_line = strlen(line);
            {
                size_t i;
                for (i = 0; i < strlen(line); i++) {
                    if ('\t' == line[i])
                        sz_line += 7;
                }
            }

            if (strlen(line) > coru_stats_width(stats)) {
                coru_stats_set_width(stats, sz_line);
            }

            coru_stats_set_height(stats, coru_stats_height(stats) + 1);
        }
    }

    free(line);
    fclose(fp);

    line = NULL;
    fp = NULL;

#if DEBUG
    PUTS("Source width: %lu", coru_stats_width(stats));
    PUTS("Source height: %lu", coru_stats_height(stats));
    /* Add stats for non-comment lines. */
#endif

    comment_single_start = _init_comment_single_start();
    if (!comment_single_start)
        goto ERROR_LOAD;

    comment_single_end = _init_comment_single_end();
    if (!comment_single_end)
        goto ERROR_LOAD;

    comment_multiple_start = _init_comment_multiple_start();
    if (!comment_multiple_start)
        goto ERROR_LOAD;

    comment_multiple_end = _init_comment_multiple_end();
    if (!comment_multiple_end)
        goto ERROR_LOAD;

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

    size_t width_number = indent \
        + strlen(hash_table_get(comment_single_start, language_to_string(lang))) \
        + space + digit;

    if (0 != strcmp("", hash_table_get(comment_single_end, language_to_string(lang)))) {
        width_number += space \
            + strlen(hash_table_get(comment_single_end, language_to_string(lang)));
    }

    size_t width_new = coru_stats_width(stats) + width_number \
        + 1;  /* Trailing zero. */

#if DEBUG
    PUTS("Destination width: %lu", width_new);
#endif

    *out = (char *) malloc(coru_stats_height(stats) * width_new * sizeof(char));
    if (!out) {
        PUTERR("Failed to allocate memory for output");
        PUTERR("Check available system memory");
        goto ERROR_LOAD;
    }

    (*out)[0] = '\0';  /* Strip down the string to zero. */

    fp = fopen(coru_argument_path(arg), "r");
    if (!fp) {
    #if DEBUG
        PUTERR("Failed to open file at %s", coru_argument_path(arg));
    #endif
        goto ERROR_LOAD;
    }

    line = (char *) malloc(line_size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate line object");
        goto ERROR_LOAD;
    }

    size_t line_number = 0;
    size_t digit_line_number;
    size_t multi = 0;
    char *lang_string = language_to_string(lang);
    char *multi_start = hash_table_get(comment_multiple_start, lang_string);
    char *multi_end = hash_table_get(comment_multiple_end, lang_string);
    while (fgets(line, line_size, fp)) {
        size_t sz_space;
        size_t sz_start;
        size_t sz_end;
        if (line_size == strlen(line)) {
            if ('\n' != line[line_size - 1]) {
                line_size <<= 1;
                if (!realloc(line, line_size)) {
                    PUTERR("Failed to realloc line buffer object");
                    PUTERR("Check available system memory");
                    goto ERROR_LOAD;
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
            sz_start = strlen(
                hash_table_get(comment_single_start, lang_string));
            strncat(*out, hash_table_get(comment_single_start, lang_string), sz_start);

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
                goto ERROR_LOAD;
            }

            if (sprintf(num_s, "%lu", line_number) < 0) {
                PUTERR("Failed to insert a number");
                goto ERROR_LOAD;
            }

            strncat(*out, num_s, strlen(num_s) + 1);

            free(num_s);

            if (0 != strcmp("",
                hash_table_get(comment_single_end, lang_string))) {
                /* Insert a space. */
                strncat(*out, " ", 1 + 1);

                /* Insert the end word of single line comment. */
                sz_end = strlen(
                    hash_table_get(comment_single_end, lang_string));
                strncat(*out, hash_table_get(comment_single_end, lang_string),
                    sz_end + 1);
            }

            /* Insert EOL. */
            strncat(*out, END_OF_LINE, strlen(END_OF_LINE) + 1);
        }
    }

    /* Free system resources. */
    free(line);
    fclose(fp);
    hash_table_delete(comment_multiple_end);
    hash_table_delete(comment_multiple_start);
    hash_table_delete(comment_single_end);
    hash_table_delete(comment_single_start);
    coru_stats_delete((void *) stats);

    return TRUE;

ERROR_LOAD:
    if (comment_multiple_end)
        hash_table_delete(comment_multiple_end);

    if (comment_multiple_start)
        hash_table_delete(comment_multiple_start);

    if (comment_single_end)
        hash_table_delete(comment_single_end);

    if (comment_single_start)
        hash_table_delete(comment_single_start);

    if (line)
        free(line);

    if (fp)
        fclose(fp);

    if (stats)
        coru_stats_delete((void *) stats);

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
