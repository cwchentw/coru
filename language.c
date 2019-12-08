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

    language_t lang = LANGUAGE_UNKNOWN;

    char *sp = path;
    char *ext = (char *) malloc(sz);
    if (!ext) {
        PUTERR("Failed to allocate line buffer");
        PUTERR("Check available system memory");
        return LANGUAGE_UNKNOWN;
    }

    snprintf(ext, sz, "%s", sp + index);

    if (is_string_equal(".c", ext)) {
        lang = LANGUAGE_C;
    }
    else if (is_string_equal(".h", ext)) {
        lang = LANGUAGE_C;
    }
    else if (is_string_equal(".cpp", ext)) {
        lang = LANGUAGE_CPP;
    }
    else if (is_string_equal(".cxx", ext)) {
        lang = LANGUAGE_CPP;
    }
    else if (is_string_equal(".cc", ext)) {
        lang = LANGUAGE_CPP;
    }
    else if (is_string_equal(".hpp", ext)) {
        lang = LANGUAGE_CPP;
    }
    else if (is_string_equal(".m", ext)) {
        lang = LANGUAGE_OBJC;
    }
    else if (is_string_equal(".mm", ext)) {
        lang = LANGUAGE_OBJCPP;
    }
    else if (is_string_equal(".java", ext)) {
        lang = LANGUAGE_JAVA;
    }
    else if (is_string_equal(".cs", ext)) {
        lang = LANGUAGE_CSHARP;
    }
    else if (is_string_equal(".pl", ext)) {
        lang = LANGUAGE_PERL;
    }
    else if (is_string_equal(".py", ext)) {
        lang = LANGUAGE_PYTHON;
    }
    else if (is_string_equal(".rb", ext)) {
        lang = LANGUAGE_RUBY;
    }
    else if (is_string_equal(".php", ext)) {
        lang = LANGUAGE_PHP;
    }
    else if (is_string_equal(".csh", ext)) {
        lang = LANGUAGE_CSH;
    }
    else if (is_string_equal(".sh", ext)) {
        lang = LANGUAGE_SH;
    }

    if (lang != LANGUAGE_UNKNOWN) {
        free(ext);
        return lang;
    }

    free(ext);

    FILE *fp = NULL;
    char *line = NULL;

    fp = fopen(path, "r");
    if (!fp) {
    #if DEBUG
        PUTERR("Unable to load file: %s", path);
    #endif
        goto ERROR;
    }

    size_t size = 150;  /* A sensible default line width. */
    line = (char *) malloc(size * sizeof(char));
    if (!line) {
    #if DEBUG
        PUTERR("Failed to allocate line");
    #endif
        goto ERROR;
    }

    while(fgets(line, size, fp)) {
        if (size == strlen(line)) {
            if ('\n' != line[size-1]) {
                /* Double the size of the buffer. */
                size <<= 1;
                line = realloc(line, size);
                if (!line) {
                #if DEBUG
                    PUTERR("Failed to reallocate line");
                #endif
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

            /* We only parse the first line of target source. */
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
    case LANGUAGE_JAVA:
        return STRING_JAVA;
    case LANGUAGE_CSHARP:
        return STRING_CSHARP;
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
