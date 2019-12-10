#include "language.h"
#include "print_function.h"
#include "syntax_data.h"

hash_table_t * init_comment_single_start(void)
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
        "#",   /* Perl */
        "#",   /* Python */
        "#",   /* Ruby */
        "/*",  /* PHP */
        "/*",  /* JavaScript */
        "/*",  /* Golang */
        "/*",  /* Rust */
        "/*",  /* ObjC */
        "/*",  /* ObjC++ */
        "/*",  /* Swift */
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

hash_table_t * init_comment_single_end(void)
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

hash_table_t * init_comment_multiple_start(void)
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

hash_table_t * init_comment_multiple_end(void)
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
