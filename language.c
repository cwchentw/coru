#include <stdio.h>
#include <stdlib.h>
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
    else if (is_string_equal(".hpp", ext)) {
        return LANGUAGE_CPP;
    }
    else if (is_string_equal(".m", ext)) {
        return LANGUAGE_OBJC;
    }
    else if (is_string_equal(".mm", ext)) {
        return LANGUAGE_OBJCPP;
    }
    else if (is_string_equal(".pl", ext)) {
        return LANGUAGE_PERL;
    }
    else if (is_string_equal(".py", ext)) {
        return LANGUAGE_PYTHON;
    }
    else if (is_string_equal(".rb", ext)) {
        return LANGUAGE_RUBY;
    }
    else if (is_string_equal(".php", ext)) {
        return LANGUAGE_PHP;
    }
    else if (is_string_equal(".csh", ext)) {
        return LANGUAGE_CSH;
    }
    else if (is_string_equal(".sh", ext)) {
        return LANGUAGE_SH;
    }

    FILE *fp = NULL;
    char *line = NULL;
    language_t lang = LANGUAGE_UNKNOWN;

    fp = fopen(path, "r");
    if (!fp) {
        PUTERR("Unable to load file: %s", path);
        goto ERROR;
    }

    size_t size = 150;
    line = (char *) malloc(size * sizeof(char));
    if (!line) {
        PUTERR("Failed to allocate line");
        goto ERROR;
    }

    while(fgets(line, size, fp)) {
        if (size == strlen(line)) {
            if ('\n' != line[size-1]) {
                /* Double the size of the buffer. */
                size <<= 1;
                line = realloc(line, size);
                if (!line) {
                    PUTERR("Failed to reallocate line");
                    goto ERROR;
                }
            } else {
                goto PARSE_LINE;
            }
        }
        else {
PARSE_LINE:
            if (string_starts_with(line, "#!")) {
                if (string_contains(line, "perl")) {
                    lang = LANGUAGE_PERL;
                }
                else if (string_contains(line, "python")) {
                    lang = LANGUAGE_PYTHON;
                }
                else if (string_contains(line, "ruby")) {
                    lang = LANGUAGE_RUBY;
                }
                else if (string_contains(line, "php")) {
                    lang = LANGUAGE_PHP;
                }
                else if (string_contains(line, "csh")) {
                    lang = LANGUAGE_CSH;
                }
                else if (string_contains(line, "sh")) {
                    lang = LANGUAGE_SH;
                }
                else {
                    lang = LANGUAGE_UNKNOWN;
                }
            }
            else {
                lang = LANGUAGE_UNKNOWN;
            }

            break;
        }
    }

    free(line);
    fclose(fp);

    return lang;

ERROR:
    if (line)
        free(line);

    if (fp)
        fclose(fp);

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
        return STRING_C;
    case LANGUAGE_CPP:
        return STRING_CPP;
    case LANGUAGE_OBJC:
        return STRING_OBJC;
    case LANGUAGE_OBJCPP:
        return STRING_OBJCPP;
    case LANGUAGE_PERL:
        return STRING_PERL;
    case LANGUAGE_PYTHON:
        return STRING_PYTHON;
    case LANGUAGE_RUBY:
        return STRING_RUBY;
    case LANGUAGE_PHP:
        return STRING_PHP;
    case LANGUAGE_CSH:
        return STRING_CSH;
    case LANGUAGE_SH:
        return STRING_SH;
    default:
        return "";
    }
}
