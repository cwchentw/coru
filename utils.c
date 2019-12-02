#include <stdio.h>
#include <string.h>
#include "boolean.h"
#include "language.h"
#include "utils.h"


BOOL is_command_equal(COMMAND a, COMMAND b)
{
    return a == b;
}

BOOL is_string_equal(char *a, char *b)
{
    if (0 == strcmp(a, b))
        return TRUE;

    return FALSE;
}
