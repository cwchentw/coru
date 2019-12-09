#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "coru_argument.h"
#include "boolean.h"
#include "language.h"
#include "utils.h"

struct coru_argument_t {
    int argc;
    char **argv;
    int index;
    language_t lang;
    CORU_COMMAND cmd;
    char *path;
};

static BOOL _is_c(char *opt);
static BOOL _is_cpp(char *opt);
static BOOL _is_java(char *opt);
static BOOL _is_csharp(char *opt);

coru_argument_t * coru_argument_parse(int argc, char **argv)
{
    coru_argument_t *arg = (coru_argument_t *) malloc(sizeof(coru_argument_t));
    if (!arg) {
        PUTERR("Failed to allocate memory for argument object");
        PUTERR("Check available system memory");
        return arg;
    }

    arg->argc = argc;
    arg->argv = argv;
    arg->index = 1;
    arg->lang = LANGUAGE_UNKNOWN;
    arg->cmd = CORU_COMMAND_UNKNOWN;
    arg->path = NULL;

    if (argc < 2) {
        arg->cmd = CORU_COMMAND_TOO_FEW;
        return arg;
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
        else if (_is_c(opt)) {
            arg->lang = LANGUAGE_C;
            arg->index += 1;
        }
        else if (_is_cpp(opt)) {
            arg->lang = LANGUAGE_CPP;
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

    return arg;
}

static BOOL _is_c(char *opt)
{
    return 0 == strcmp(opt, "-c") || 0 == strcmp(opt, "--as-c");
}

static BOOL _is_cpp(char *opt)
{
    return 0 == strcmp(opt, "-cpp") || 0 == strcmp(opt, "-cxx")
        || 0 == strcmp(opt, "-c++") || 0 == strcmp(opt, "--as-cpp")
        || 0 == strcmp(opt, "--as-cxx") || 0 == strcmp(opt, "--as-c++");
}

static BOOL _is_java(char *opt)
{
    return 0 == strcmp(opt, "-java") || 0 == strcmp(opt, "--as-java");
}

static BOOL _is_csharp(char *opt)
{
    return 0 == strcmp(opt, "-c#") || 0 == strcmp(opt, "-csharp")
        || 0 == strcmp(opt, "--as-c#") || 0 == strcmp(opt, "--as-csharp");
}

void coru_argument_delete(void *self)
{
    assert(self);

    free(self);
}

language_t coru_argument_language(coru_argument_t *self)
{
    assert(self);

    return self->lang;
}

CORU_COMMAND coru_argument_command(coru_argument_t *self)
{
    assert(self);

    return self->cmd;
}

char * coru_argument_path(coru_argument_t *self)
{
    assert(self);

    return self->path;
}
