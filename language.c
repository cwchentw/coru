#include <string.h>
#include "boolean.h"
#include "language.h"
#include "utils.h"

language_t detect_target_language(char *path)
{
    size_t sz = strlen(path);

    size_t index = 0;
    {
        size_t i;
        for (i = sz - 1; i > 0; i--) {
            if ('.' == path[i]) {
                index = i;
                break;
            }
        }
    }

    if (index == 0)
        return LANGUAGE_UNKNOWN;

    #define BUF_SIZE 100
    char *sp = path;
    char ext[BUF_SIZE];

    snprintf(ext, BUF_SIZE, "%s", sp + index);

    if (is_string_equal(".c", ext)) {
        return LANGUAGE_C;
    }
    else if (is_string_equal(".h", ext)) {
        return LANGUAGE_C;
    }
    else if (is_string_equal(".cpp", ext)) {
        return LANGUAGE_CPP;
    }
    else if (is_string_equal(".cxx", ext)) {
        return LANGUAGE_CPP;
    }
    else if (is_string_equal(".cc", ext)) {
        return LANGUAGE_CPP;
    }

    return LANGUAGE_UNKNOWN;
}

BOOL is_language_equal(language_t a, language_t b)
{
    return a == b;
}

char * language_to_string(language_t lang)
{
    switch (lang) {
    case LANGUAGE_C:
        return "C";
    case LANGUAGE_CPP:
        return "C++";
    case LANGUAGE_OBJC:
        return "Objective-C";
    default:
        return "";
    }
}
