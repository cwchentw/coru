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

#include "argument.h"
#include "boolean.h"
#include "command.h"
#include "hash_table.h"
#include "help.h"
#include "language.h"
#include "metadata.h"
#include "stats.h"
#include "utils.h"

static BOOL coru_run_load(coru_argument_t * arg, char *out);

BOOL coru_run(coru_argument_t * arg, char *out)
{
    if (is_command_equal(coru_argument_command(arg), COMMAND_VERSION)) {
        help_version();
        return TRUE;
    }
    else if (is_command_equal(coru_argument_command(arg), COMMAND_LICENSE)) {
        help_license();
        return TRUE;
    }
    else if (is_command_equal(coru_argument_command(arg), COMMAND_HELP)) {
        help_help(stdout);
        return TRUE;
    }
    else if (is_command_equal(coru_argument_command(arg), COMMAND_TOO_FEW)) {
        PUTERR("No input file");
        return FALSE;
    }
    else if (is_command_equal(coru_argument_command(arg), COMMAND_LOAD)) {
        if (!coru_run_load(arg, out)) {
            PUTERR("Failed to load target file");
            return FALSE;
        }

        return TRUE;
    }
    else if (is_command_equal(coru_argument_command(arg), COMMAND_TOO_MANY)) {
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

static BOOL coru_run_load(coru_argument_t * arg, char *out)
{
    coru_stats_t *stats = NULL;
    FILE *fp = NULL;
    char *line = NULL;
    hash_table_t *comment_single_start = NULL;
    hash_table_t *comment_single_end = NULL;
    hash_table_t *comment_multiple_start = NULL;

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

    size_t line_size = 150;	 /* Sensible line size */
    line = (char *) malloc(line_size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate line object");
        goto ERROR_LOAD;
    }

    while (fgets(line, line_size, fp)) {
        if (line_size == strlen(line)) {
            if ('\n' != line[line_size - 1]) {
                line_size <<= 1;
                if (!realloc(line, line_size)) {
                    PUTERR("Failed to realloc line object");
                    goto ERROR_LOAD;
                }
            } else
                goto LOAD_LINE;
        } else {
LOAD_LINE:
            if (strlen(line) > coru_stats_width(stats)) {
                coru_stats_set_width(stats, strlen(line));
            }

            coru_stats_set_height(stats, coru_stats_height(stats) + 1);
        }
    }

#if DEBUG
    PUTS("Source width: %lu", coru_stats_width(stats));
    PUTS("Source height: %lu", coru_stats_height(stats));
    /* Add stats for non-comment lines. */
#endif

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
    comment_single_start = _init_comment_single_start();
    if (!comment_single_start) {
    #if DEBUG
        PUTERR("Failed to init comment single start table");
    #endif
        goto ERROR_LOAD;
    }

    comment_single_end = _init_comment_single_end();
    if (!comment_single_end) {
    #if DEBUG
        PUTERR("Failed to init comment single end table");
    #endif
        goto ERROR_LOAD;
    }

    comment_multiple_start = _init_comment_multiple_start();
    if (!comment_multiple_start) {
    #if DEBUG
        PUTERR("Failed to init comment multiple start table");
    #endif
        goto ERROR_LOAD;
    }

    /* Free system resources. */
    hash_table_delete(comment_multiple_start);
    hash_table_delete(comment_single_end);
    hash_table_delete(comment_single_start);
    free(line);
    fclose(fp);
    coru_stats_delete((void *) stats);

    return TRUE;

ERROR_LOAD:
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
        STRING_OBJC,
        STRING_OBJCPP,
        STRING_JAVA,
        STRING_CSHARP,
        STRING_PERL,
        STRING_PYTHON,
        STRING_RUBY,
        STRING_PHP,
        STRING_SWIFT,
        STRING_GO,
        STRING_RUST,
        STRING_CSH,
        STRING_SH
    };

    char *values[] = {
        "/*",  /* C */
        "//",  /* C++ */
        "/*",  /* ObjC */
        "/*",  /* ObjC++ */
        "//",  /* Java */
        "//",  /* C# */
        "#",   /* Perl */
        "#",   /* Python */
        "#",   /* Ruby */
        "#",   /* PHP */
        "//",  /* Swift */
        "//",  /* Golang */
        "//",  /* Rust */
        "#",   /* C shell */
        "#"    /* POSIX shell */
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
        STRING_OBJC,
        STRING_OBJCPP,
        STRING_JAVA,
        STRING_CSHARP,
        STRING_PERL,
        STRING_PYTHON,
        STRING_RUBY,
        STRING_PHP,
        STRING_SWIFT,
        STRING_GO,
        STRING_RUST,
        STRING_CSH,
        STRING_SH
    };

    char *values[] = {
        "*/",  /* C */
        "",    /* C++ */
        "*/",  /* ObjC */
        "*/",  /* ObjC++ */
        "",    /* Java */
        "",    /* C# */
        "",    /* Perl */
        "",    /* Python */
        "",    /* Ruby */
        "",    /* PHP */
        "",    /* Swift */
        "",    /* Golang */
        "",    /* Rust */
        "",    /* C shell */
        ""     /* POSIX shell */
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
        STRING_OBJC,
        STRING_OBJCPP,
        STRING_JAVA,
        STRING_CSHARP,
        STRING_PERL,
        STRING_PYTHON,
        STRING_RUBY,
        STRING_PHP,
        STRING_SWIFT,
        STRING_GO,
        STRING_RUST,
        STRING_CSH,
        STRING_SH
    };

    char *values[] = {
        "/*",  /* C */
        "/*",  /* C++ */
        "/*",  /* ObjC */
        "/*",  /* ObjC++ */
        "/*",  /* Java */
        "/*",  /* C# */
        "",    /* Perl */
        "",    /* Python */
        "",    /* Ruby */
        "/*",  /* PHP */
        "/*",  /* Swift */
        "/*",  /* Golang */
        "/*",  /* Rust */
        "",    /* C shell */
        ""     /* POSIX shell */
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
