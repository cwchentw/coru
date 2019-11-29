#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "boolean.h"

BOOL is_command_equal(COMMAND a, COMMAND b)
{
    return a == b;
}

char * detect_target_language(char *path)
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
        return "";

    #define BUF_SIZE 100
    char *sp = path;
    char ext[BUF_SIZE];
    
    snprintf(ext, BUF_SIZE, "%s", sp + index);

    #define LANG_C   "C"
    #define LANG_CPP "C++"
    if (is_string_equal(".c", ext)) {
        return LANG_C;
    }
    else if (is_string_equal(".h", ext)) {
        return LANG_C;
    }
    else if (is_string_equal(".cpp", ext)) {
        return LANG_CPP;
    }
    else if (is_string_equal(".cxx", ext)) {
        return LANG_CPP;
    }
    else if (is_string_equal(".cc", ext)) {
        return LANG_CPP;
    }

    return "";
}

BOOL is_string_equal(char *a, char *b)
{
    if (0 == strcmp(a, b))
        return TRUE;

    return FALSE;
}
