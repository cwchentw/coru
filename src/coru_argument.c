#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "coru.h"
#include "coru_argument.h"
#include "language.h"
#include "as_language.h"
#include "print.h"

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
        as_language(arg, opt)  /* A set of language selection else if block */
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
