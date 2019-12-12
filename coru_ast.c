#include "coru_ast.h"

typedef struct coru_ast_code_t coru_ast_code_t;
typedef struct coru_ast_tab_t coru_ast_tab_t;
typedef struct coru_ast_backslash_t coru_ast_backslash_t;
typedef struct coru_ast_string_t coru_ast_string_t;

struct coru_ast_t {
    CORU_AST_TYPE ast_t;
    union {
        coru_ast_code_t *ast_code_t;
        coru_ast_tab_t *ast_tab_t;
        coru_ast_backslash_t *ast_backslash_t;
        coru_ast_string_t *ast_string_t;
    };
};

struct coru_ast_code_t {
    /* Declare it later. */
};

struct coru_ast_tab_t {
    /* Declare it later. */
};

struct coru_ast_backslash_t {
    /* Declare it later. */
};

struct coru_ast_string_t {
    /* Declare it later. */
};
