#include "language.h"
#include "print.h"
#include "syntax_data.h"

/* Comment data as singleton objects. */
hash_table_t *comment_single_start = NULL;
hash_table_t *comment_single_end = NULL;
hash_table_t *comment_multiple_start = NULL;
hash_table_t *comment_multiple_end = NULL;

hash_table_t * init_comment_single_start(void)
{
    hash_table_t *table = hash_table_new();
    if (!table)
        return table;

    char *keys[] = {
        STRING_C,
        STRING_CPP,
        STRING_PASCAL,
        STRING_FORTRAN,
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
        STRING_CMAKE,
        STRING_MAKE
    };

    char *values[] = {
        "/*",   /* C */
        "/*",   /* C++ */
        "(*",   /* Object Pascal */
        "!",    /* Fortran */
        "/*",   /* Java */
        "/*",   /* C# */
        "#",    /* Perl */
        "#",    /* Python */
        "#",    /* Ruby */
        "/*",   /* PHP */
        "/*",   /* JavaScript */
        "/*",   /* Golang */
        "/*",   /* Rust */
        "/*",   /* ObjC */
        "/*",   /* ObjC++ */
        "/*",   /* Swift */
        "#",    /* C shell */
        "#",    /* Bourne shell */
        "#[[",  /* CMake */
        "#"     /* Make */
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
        STRING_CMAKE,
        STRING_MAKE
    };

    char *values[] = {
        "*/",  /* C */
        "*/",  /* C++ */
        "*)",  /* Object Pascal */
        "",    /* Fortran */
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
        STRING_CMAKE,
        STRING_MAKE
    };

    char *values[] = {
        "/*",   /* C */
        "/*",   /* C++ */
        "(*",   /* Object Pascal */
        "",     /* Fortran */
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
        STRING_CMAKE,
        STRING_MAKE
    };

    char *values[] = {
        "*/",  /* C */
        "*/",  /* C++ */
        "*)",  /* Object Pascal */
        "",    /* Fortran */
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
