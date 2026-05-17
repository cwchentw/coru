#include <string.h>
#include "language.h"
#include "print.h"
#include "syntax_data.h"

#define array_length(arr)  ((sizeof(arr)) / (sizeof(arr[0])))


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
    { STRING_SWIFT, "/*" },   /* Swift */
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

static comment_text_t comment_single_end[] = {
    { STRING_C, "*/" },       /* C */
    { STRING_CPP, "*/" },     /* C++ */
    { STRING_PASCAL, "*)" },  /* Object Pascal */
    { STRING_FORTRAN, "" },   /* Fortran */
    { STRING_CL, "" },        /* Common Lisp */
    { STRING_JAVA, "*/" },    /* Java */
    { STRING_CSHARP, "*/" },  /* C# */
    { STRING_PERL, "" },      /* Perl */
    { STRING_PYTHON, "" },    /* Python */
    { STRING_RUBY, "" },      /* Ruby */
    { STRING_PHP, "*/" },     /* PHP */
    { STRING_JS, "*/" },      /* JavaScript */
    { STRING_GO, "*/" },      /* Golang */
    { STRING_RUST, "*/" },    /* Rust */
    { STRING_OBJC, "*/" },    /* Objective-C */
    { STRING_OBJCPP, "*/" },  /* Objective-C++ */
    { STRING_SWIFT, "*/" },   /* Swift */
    { STRING_CSH, "" },       /* C shell */
    { STRING_SH, "" },        /* Bourne shell */
    { STRING_PS, "#>" },      /* PowerShell */
    { STRING_CMAKE, "]]" },   /* CMake */
    { STRING_MAKE, "" }       /* Make */
};

const char * single_end_string(const char *key)
{
    size_t i;
    for (i = 0; i < array_length(comment_single_end); ++i) {
        if (strcmp(comment_single_end[i].key, key) == 0)
            return comment_single_end[i].value;
    }

    return NULL;
}

static comment_text_t comment_multiple_start[] = {
    { STRING_C, "/*" },       /* C */
    { STRING_CPP, "/*" },     /* C++ */
    { STRING_PASCAL, "(*" },  /* Object Pascal */
    { STRING_FORTRAN, "" },   /* Fortran */
    { STRING_CL, "#|" },      /* Common Lisp */
    { STRING_JAVA, "/*" },    /* Java */
    { STRING_CSHARP, "/*" },  /* C# */
    { STRING_PERL, "" },      /* Perl */
    { STRING_PYTHON, "" },    /* Python */
    { STRING_RUBY, "" },      /* Ruby */
    { STRING_PHP, "/*" },     /* PHP */
    { STRING_JS, "/*" },      /* JavaScript */
    { STRING_GO, "/*" },      /* Golang */
    { STRING_RUST, "/*" },    /* Rust */
    { STRING_OBJC, "/*" },    /* Objective-C */
    { STRING_OBJCPP, "/*" },  /* Objective-C++ */
    { STRING_SWIFT, "/*" },   /* Swift */
    { STRING_CSH, "" },       /* C shell */
    { STRING_SH, "" },        /* Bourne shell */
    { STRING_PS, "<#" },      /* PowerShell */
    { STRING_CMAKE, "#[[" },  /* CMake */
    { STRING_MAKE, "" }       /* Make */
};

const char * multiple_start_string(const char *key)
{
    size_t i;
    for (i = 0; i < array_length(comment_multiple_start); ++i) {
        if (strcmp(comment_multiple_start[i].key, key) == 0)
            return comment_multiple_start[i].value;
    }

    return NULL;
}

static comment_text_t comment_multiple_end[] = {
    { STRING_C, "*/" },       /* C */
    { STRING_CPP, "*/" },     /* C++ */
    { STRING_PASCAL, "*)" },  /* Object Pascal */
    { STRING_FORTRAN, "" },   /* Fortran */
    { STRING_CL, "|#" },      /* Common Lisp */
    { STRING_JAVA, "*/" },    /* Java */
    { STRING_CSHARP, "*/" },  /* C# */
    { STRING_PERL, "" },      /* Perl */
    { STRING_PYTHON, "" },    /* Python */
    { STRING_RUBY, "" },      /* Ruby */
    { STRING_PHP, "*/" },     /* PHP */
    { STRING_JS, "*/" },      /* JavaScript */
    { STRING_GO, "*/" },      /* Golang */
    { STRING_RUST, "*/" },    /* Rust */
    { STRING_OBJC, "*/" },    /* Objective-C */
    { STRING_OBJCPP, "*/" },  /* Objective-C++ */
    { STRING_SWIFT, "*/" },   /* Swift */
    { STRING_CSH, "" },       /* C shell */
    { STRING_SH, "" },        /* Bourne shell */
    { STRING_PS, "#>" },      /* PowerShell */
    { STRING_CMAKE, "]]" },   /* CMake */
    { STRING_MAKE, "" }       /* Make */
};

const char * multiple_end_string(const char *key)
{
    size_t i;
    for (i = 0; i < array_length(comment_multiple_end); ++i) {
        if (strcmp(comment_multiple_end[i].key, key) == 0)
            return comment_multiple_end[i].value;
    }

    return NULL;
}
