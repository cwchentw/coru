#include <string.h>
#include "language.h"
#include "print.h"
#include "syntax_data.h"

#define array_length(arr)  ((sizeof(arr)) / (sizeof(arr[0])))

/* Comment data as singleton objects. */
hash_table_t *comment_single_end = NULL;
hash_table_t *comment_multiple_start = NULL;
hash_table_t *comment_multiple_end = NULL;

typedef struct comment_text_t {
    const char *key;
    const char *value;
} comment_text_t;

static comment_text_t comment_single_start[] = {
    { STRING_C, "/*" },       /* C */
    { STRING_CPP, "/*" },     /* C++ */
    { STRING_PASCAL, "(*" },  /* Object Pascal */
    { STRING_FORTRAN, "!" },  /* Fortran */
    { STRING_CL, ";" },       /* Common Lisp */
    { STRING_JAVA, "/*" },    /* Java */
    { STRING_CSHARP, "/*" },  /* C# */
    { STRING_PERL, "#" },     /* Perl */
    { STRING_PYTHON, "#" },   /* Python */
    { STRING_RUBY, "#" },     /* Ruby */
    { STRING_PHP, "/*" },     /* PHP */
    { STRING_JS, "/*" },      /* JavaScript */
    { STRING_GO, "/*" },      /* Golang */
    { STRING_RUST, "/*" },    /* Rust */
    { STRING_OBJC, "/*" },    /* Objective-C */
    { STRING_OBJCPP, "/*" },  /* Objective-C++ */
    { STRING_SWIFT, "/*"},    /* Swift */
    { STRING_CSH, "#" },      /* C shell */
    { STRING_SH, "#" },       /* Bourne shell */
    { STRING_PS, "<#" },      /* PowerShell */
    { STRING_CMAKE, "#[[" },  /* CMake */
    { STRING_MAKE, "#" }      /* Make */
};

const char * single_start_string(const char *key)
{
    size_t i;
    for (i = 0; i < array_length(comment_single_start); ++i) {
        if (strcmp(comment_single_start[i].key, key) == 0)
            return comment_single_start[i].value;
    }

    return NULL;
}

hash_table_t * init_comment_single_end(void)
{
    hash_table_t *table = hash_table_new();
    if (!table)
        return table;

    char *keys[] = {
        STRING_C,
        STRING_CPP,
        STRING_PASCAL,
        STRING_FORTRAN,
        STRING_CL,
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
        STRING_SH,
        STRING_PS,
        STRING_CMAKE,
        STRING_MAKE
    };

    char *values[] = {
        "*/",  /* C */
        "*/",  /* C++ */
        "*)",  /* Object Pascal */
        "",    /* Fortran */
        "",    /* Common Lisp */
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
        "",    /* Bourne shell */
        "#>",  /* PowerShell */
        "]]",  /* CMake */
        ""     /* Make */
    };

    {
        size_t i;
        for (i = 0; i < sizeof(keys) / sizeof(char *); i++) {
            if (!hash_table_add(table, keys[i], values[i])) {
            #if DEBUG
                PUTERR("Failed to add key-value to the hash table");
            #endif
                goto ERROR_HASH_TABLE;
            }
        }
    }

    return table;

ERROR_HASH_TABLE:
    if (table)
        hash_table_delete(table);

    return NULL;
}

hash_table_t * init_comment_multiple_start(void)
{
    hash_table_t *table = hash_table_new();
    if (!table)
        return table;

    char *keys[] = {
        STRING_C,
        STRING_CPP,
        STRING_PASCAL,
        STRING_FORTRAN,
        STRING_CL,
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
        STRING_SH,
        STRING_PS,
        STRING_CMAKE,
        STRING_MAKE
    };

    char *values[] = {
        "/*",   /* C */
        "/*",   /* C++ */
        "(*",   /* Object Pascal */
        "",     /* Fortran */
        "#|",   /* Common Lisp */
        "/*",   /* Java */
        "/*",   /* C# */
        "",     /* Perl */
        "",     /* Python */
        "",     /* Ruby */
        "/*",   /* PHP */
	    "/*",   /* JavaScript */
        "/*",   /* Golang */
        "/*",   /* Rust */
        "/*",   /* ObjC */
        "/*",   /* ObjC++ */
        "/*",   /* Swift */
        "",     /* C shell */
        "",     /* Bourne shell */
        "<#",   /* PowerShell */
        "#[[",  /* CMake */
        ""      /* Make */
    };

    {
        size_t i;
        for (i = 0; i < sizeof(keys) / sizeof(char *); i++) {
            if (!hash_table_add(table, keys[i], values[i])) {
            #if DEBUG
                PUTERR("Failed to add key-value to the hash table");
            #endif
                goto ERROR_HASH_TABLE;
            }
        }
    }

    return table;

ERROR_HASH_TABLE:
    if (table)
        hash_table_delete(table);

    return NULL;
}

hash_table_t * init_comment_multiple_end(void)
{
    hash_table_t *table = hash_table_new();
    if (!table)
        return table;

    char *keys[] = {
        STRING_C,
        STRING_CPP,
        STRING_PASCAL,
        STRING_FORTRAN,
        STRING_CL,
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
        STRING_SH,
        STRING_PS,
        STRING_CMAKE,
        STRING_MAKE
    };

    char *values[] = {
        "*/",  /* C */
        "*/",  /* C++ */
        "*)",  /* Object Pascal */
        "",    /* Fortran */
        "|#",  /* Common Lisp */
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
        "",    /* Bourne shell */
        "#>",  /* PowerShell */
        "]]",  /* CMake */
        ""     /* Make */
    };

    {
        size_t i;
        for (i = 0; i < sizeof(keys) / sizeof(char *); i++) {
            if (!hash_table_add(table, keys[i], values[i])) {
            #if DEBUG
                PUTERR("Failed to add key-value to the hash table");
            #endif
                goto ERROR_HASH_TABLE;
            }
        }
    }

    return table;

ERROR_HASH_TABLE:
    if (table)
        hash_table_delete(table);

    return NULL;
}
