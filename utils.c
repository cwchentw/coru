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

BOOL string_starts_with(char *a, char *b)
{
    size_t sz_a = strlen(a);
    size_t sz_b = strlen(b);

    if (sz_a < sz_b)
        return FALSE;

    size_t i = 0;
    while (i < sz_b) {
        if (a[i] != b[i])
            return FALSE;

        i++;
    }

    return TRUE;
}

BOOL string_contains(char *a, char *b)
{
    size_t sz_a = strlen(a);
    size_t sz_b = strlen(b);

    if (sz_a < sz_b)
        return FALSE;

    size_t i = 0;
    while (i + sz_b <= sz_a) {
        size_t _i = i;
        size_t _j = 0;

        BOOL matched = TRUE;
        while (_j < sz_b) {
            if (a[_i] != b[_j]) {
                matched = FALSE;
                break;
            }

            _i++;
            _j++;
        }

        if (matched)
            return TRUE;

        i++;
    }

    return FALSE;
}
