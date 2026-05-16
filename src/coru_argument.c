#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "coru.h"
#include "coru_argument.h"
#include "language.h"
#include "print.h"

#define _is_c(opt) \
    (0 == strcmp((opt), "-c") || 0 == strcmp((opt), "--as-c"))
#define _is_cpp(opt) \
    (0 == strcmp((opt), "-cpp") || 0 == strcmp((opt), "-cxx") \
        || 0 == strcmp((opt), "-c++") || 0 == strcmp((opt), "--as-cpp") \
        || 0 == strcmp((opt), "--as-cxx") || 0 == strcmp((opt), "--as-c++"))
#define _is_pascal(opt) \
    (0 == strcmp((opt), "-pas") || 0 == strcmp((opt), "--as-pascal"))
#define _is_java(opt) \
    (0 == strcmp((opt), "-java") || 0 == strcmp((opt), "--as-java"))
#define _is_csharp(opt) \
    (0 == strcmp((opt), "-c#") || 0 == strcmp((opt), "-csharp") \
        || 0 == strcmp((opt), "--as-c#") || 0 == strcmp((opt), "--as-csharp"))
#define _is_perl(opt) \
    (0 == strcmp((opt), "-pl") || 0 == strcmp((opt), "-perl") \
        || 0 == strcmp((opt), "--as-perl"))
#define _is_python(opt) \
    (0 == strcmp((opt), "-py") || 0 == strcmp((opt), "-python") \
        || 0 == strcmp((opt), "--as-python"))
#define _is_ruby(opt) \
    (0 == strcmp((opt), "-rb") || 0 == strcmp((opt), "-ruby") \
        || 0 == strcmp((opt), "--as-ruby"))
#define _is_php(opt) \
    (0 == strcmp((opt), "-php") || 0 == strcmp((opt), "--as-php"))
#define _is_js(opt) \
    (0 == strcmp((opt), "-js") || 0 == strcmp((opt), "--as-js"))
#define _is_golang(opt) \
    (0 == strcmp((opt), "-go") || 0 == strcmp((opt), "-golang") \
        || 0 == strcmp((opt), "--as-golang"))
#define _is_rust(opt) \
    (0 == strcmp((opt), "-rs") || 0 == strcmp((opt), "-rust") \
        || 0 == strcmp((opt), "--as-rust"))
#define _is_objc(opt) \
    (0 == strcmp((opt), "-objc") || 0 == strcmp((opt), "--as-objc"))
#define _is_swift(opt) \
    (0 == strcmp((opt), "-swfit") || 0 == strcmp((opt), "--as-swift"))
#define _is_csh(opt) \
    (0 == strcmp((opt), "-csh") || 0 == strcmp((opt), "--as-csh"))
#define _is_sh(opt) \
    (0 == strcmp((opt), "-sh") || 0 == strcmp((opt), "--as-sh"))
#define _is_cmake(opt) \
    (0 == strcmp((opt), "-cmake") || 0 == strcmp((opt), "--as-cmake"))
#define _is_make(opt) \
    (0 == strcmp((opt), "-make") || 0 == strcmp((opt), "--as-make"))

int coru_argument_parse(coru_argument_t *arg, int argc, char **argv)
{
    if (!arg) return -1;

    arg->argc = argc;
    arg->argv = argv;
    arg->index = 1;
    arg->is_all = FALSE;
    arg->lang = LANGUAGE_UNKNOWN;
    arg->cmd = CORU_COMMAND_UNKNOWN;
    arg->path = NULL;

    if (argc < 2) {
        arg->cmd = CORU_COMMAND_TOO_FEW;
        return -1;
    }

    while (arg->index < arg->argc) {
        char *opt = arg->argv[arg->index];

        if (0 == strcmp(opt, "-v") || 0 == strcmp(opt, "--version")) {
            arg->cmd = CORU_COMMAND_VERSION;
            break;
        }
        else if (0 == strcmp(opt, "--license")) {
            arg->cmd = CORU_COMMAND_LICENSE;
            break;
        }
        else if (0 == strcmp(opt, "-h") || 0 == strcmp(opt, "--help")) {
            arg->cmd = CORU_COMMAND_HELP;
            break;
        }
        else if (0 == strcmp(opt, "--all")) {
            arg->is_all = TRUE;
            arg->index += 1;
        }
        else if (0 == strcmp(opt, "--non-empty")) {
            arg->is_all = FALSE;
            arg->index += 1;
        }
        else if (_is_c(opt)) {
            arg->lang = LANGUAGE_C;
            arg->index += 1;
        }
        else if (_is_cpp(opt)) {
            arg->lang = LANGUAGE_CPP;
            arg->index += 1;
        }
        else if (_is_pascal(opt)) {
            arg->lang = LANGUAGE_PASCAL;
            arg->index += 1;
        }
        else if (_is_java(opt)) {
            arg->lang = LANGUAGE_JAVA;
            arg->index += 1;
        }
        else if (_is_csharp(opt)) {
            arg->lang = LANGUAGE_CSHARP;
            arg->index += 1;
        }
        else if (_is_perl(opt)) {
            arg->lang = LANGUAGE_PERL;
            arg->index += 1;
        }
        else if (_is_python(opt)) {
            arg->lang = LANGUAGE_PYTHON;
            arg->index += 1;
        }
        else if (_is_ruby(opt)) {
            arg->lang = LANGUAGE_RUBY;
            arg->index += 1;
        }
        else if (_is_php(opt)) {
            arg->lang = LANGUAGE_PHP;
            arg->index += 1;
        }
        else if (_is_js(opt)) {
            arg->lang = LANGUAGE_JS;
            arg->index += 1;
        }
        else if (_is_golang(opt)) {
            arg->lang = LANGUAGE_GO;
            arg->index += 1;
        }
        else if (_is_rust(opt)) {
            arg->lang = LANGUAGE_RUST;
            arg->index += 1;
        }
        else if (_is_objc(opt)) {
            arg->lang = LANGUAGE_OBJC;
            arg->index += 1;
        }
        else if (_is_swift(opt)) {
            arg->lang = LANGUAGE_SWIFT;
            arg->index += 1;
        }
        else if (_is_csh(opt)) {
            arg->lang = LANGUAGE_CSH;
            arg->index += 1;
        }
        else if (_is_sh(opt)) {
            arg->lang = LANGUAGE_SH;
            arg->index += 1;
        }
        else if (_is_cmake(opt)) {
            arg->lang = LANGUAGE_CMAKE;
            arg->index += 1;
        }
        else if (_is_make(opt)) {
            arg->lang = LANGUAGE_MAKE;
            arg->index += 1;
        }
        else if ('-' == opt[0]) {
            arg->cmd = CORU_COMMAND_UNKNOWN;
            break;
        }
        else {
            if (CORU_COMMAND_LOAD == arg->cmd) {
                arg->cmd = CORU_COMMAND_TOO_MANY;
                break;
            }
            else {
                arg->cmd = CORU_COMMAND_LOAD;
                arg->path = opt;
                arg->index += 1;
            }
        }
    }

    if (arg->path && strstr(arg->path, "Makefile") && LANGUAGE_UNKNOWN == arg->lang)
        arg->lang = LANGUAGE_MAKE;
    else if (arg->path && strstr(arg->path, "CMakeLists.txt") && LANGUAGE_UNKNOWN == arg->lang)
        arg->lang = LANGUAGE_CMAKE;

    return 0;
}

BOOL coru_argument_is_all(const coru_argument_t *self)
{
    return self->is_all;
}

language_t coru_argument_language(const coru_argument_t *self)
{
    return self->lang;
}

CORU_COMMAND coru_argument_command(const coru_argument_t *self)
{
    return self->cmd;
}

char * coru_argument_path(const coru_argument_t *self)
{
    return self->path;
}
