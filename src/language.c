#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cstring.h"
#include "language.h"
#include "print.h"


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

    ext[0] = '\0';

#if _MSC_VER
    sprintf_s(ext, sz, "%s", sp + index);
#else
    sprintf(ext, "%s", sp + index);
#endif

    if (string_is_equal(".c", ext)) {
        lang = LANGUAGE_C;
    }
    else if (string_is_equal(".h", ext)) {
        lang = LANGUAGE_C;
    }
    else if (string_is_equal(".cpp", ext)) {
        lang = LANGUAGE_CPP;
    }
    else if (string_is_equal(".cxx", ext)) {
        lang = LANGUAGE_CPP;
    }
    else if (string_is_equal(".cc", ext)) {
        lang = LANGUAGE_CPP;
    }
    else if (string_is_equal(".hpp", ext)) {
        lang = LANGUAGE_CPP;
    }
    else if (string_is_equal(".pp", ext)) {
        lang = LANGUAGE_PASCAL;
    }
    else if (string_is_equal(".pas", ext)) {
        lang = LANGUAGE_PASCAL;
    }
    else if (string_is_equal(".f90", ext)) {
        lang = LANGUAGE_FORTRAN;
    }
    else if (string_is_equal(".lisp", ext)) {
        lang = LANGUAGE_CL;
    }
    else if (string_is_equal(".lsp", ext)) {
        lang = LANGUAGE_CL;
    }
    else if (string_is_equal(".l", ext)) {
        lang = LANGUAGE_CL;
    }
    else if (string_is_equal(".cl", ext)) {
        lang = LANGUAGE_CL;
    }
    else if (string_is_equal(".fasl", ext)) {
        lang = LANGUAGE_CL;
    }
    else if (string_is_equal(".java", ext)) {
        lang = LANGUAGE_JAVA;
    }
    else if (string_is_equal(".cs", ext)) {
        lang = LANGUAGE_CSHARP;
    }
    else if (string_is_equal(".pl", ext)) {
        lang = LANGUAGE_PERL;
    }
    else if (string_is_equal(".py", ext)) {
        lang = LANGUAGE_PYTHON;
    }
    else if (string_is_equal(".rb", ext)) {
        lang = LANGUAGE_RUBY;
    }
    else if (string_is_equal(".php", ext)) {
        lang = LANGUAGE_PHP;
    }
    else if (string_is_equal(".js", ext)) {
        lang = LANGUAGE_JS;
    }
    else if (string_is_equal(".go", ext)) {
        lang = LANGUAGE_GO;
    }
    else if (string_is_equal(".rs", ext)) {
        lang = LANGUAGE_RUST;
    }
    else if (string_is_equal(".m", ext)) {
        lang = LANGUAGE_OBJC;
    }
    else if (string_is_equal(".mm", ext)) {
        lang = LANGUAGE_OBJCPP;
    }
    else if (string_is_equal(".swift", ext)) {
        lang = LANGUAGE_SWIFT;
    }
    else if (string_is_equal(".ps1", ext)) {
        lang = LANGUAGE_PS;
    }
    else if (string_is_equal(".csh", ext)) {
        lang = LANGUAGE_CSH;
    }
    else if (string_is_equal(".sh", ext)) {
        lang = LANGUAGE_SH;
    }

    if (lang != LANGUAGE_UNKNOWN) {
        free(ext);
        return lang;
    }

    free(ext);

    FILE *fp = NULL;
    char *line = NULL;
    char *more_line = NULL;

#if _MSC_VER
    if (0 != fopen_s(&fp, path, "r")) {
        goto ERROR_LANGUAGE;
    }
#else
    fp = fopen(path, "r");
#endif
    if (!fp) {
    #if DEBUG
        PUTERR("Unable to load file: %s", path);
    #endif
        goto ERROR_LANGUAGE;
    }

    size_t size = 150;  /* A sensible default line width. */
    line = (char *) malloc(size * sizeof(char));
    if (!line) {
    #if DEBUG
        PUTERR("Failed to allocate line");
    #endif
        goto ERROR_LANGUAGE;
    }

    line[0] = '\0';

    while(fgets(line, (int) size, fp)) {
        if (size == strlen(line)) {
            if ('\n' != line[size-1]) {
                /* Double the size of the buffer. */
                size <<= 1;
                more_line = realloc(line, size);
                if (!more_line) {
                #if DEBUG
                    PUTERR("Failed to reallocate line");
                #endif
                    goto ERROR_LANGUAGE;
                }
                else {
                    line = more_line;
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
                else if (string_contains(line, "node")) {
                    lang = LANGUAGE_JS;
                }
                else if (string_contains(line, "swift")) {
                    lang = LANGUAGE_SWIFT;
                }
                else if (string_contains(line, "csh")) {
                    lang = LANGUAGE_CSH;
                }
                else if (string_contains(line, "sh")) {
                    lang = LANGUAGE_SH;
                }
                else if (string_contains(line, "instantfpc")) {
                    lang = LANGUAGE_PASCAL;
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

ERROR_LANGUAGE:
    if (line)
        free(line);

    if (fp)
        fclose(fp);

    return LANGUAGE_UNKNOWN;
}

const char * language_to_string(language_t lang)
{
    switch (lang) {
    case LANGUAGE_C:
        return STRING_C;
    case LANGUAGE_CPP:
        return STRING_CPP;
    case LANGUAGE_PASCAL:
        return STRING_PASCAL;
    case LANGUAGE_FORTRAN:
        return STRING_FORTRAN;
    case LANGUAGE_CL:
        return STRING_CL;
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
    case LANGUAGE_JS:
        return STRING_JS;
    case LANGUAGE_GO:
        return STRING_GO;
    case LANGUAGE_RUST:
        return STRING_RUST;
    case LANGUAGE_OBJC:
        return STRING_OBJC;
    case LANGUAGE_OBJCPP:
        return STRING_OBJCPP;
    case LANGUAGE_SWIFT:
        return STRING_SWIFT;
    case LANGUAGE_CSH:
        return STRING_CSH;
    case LANGUAGE_SH:
        return STRING_SH;
    case LANGUAGE_PS:
        return STRING_PS;
    case LANGUAGE_CMAKE:
        return STRING_CMAKE;
    case LANGUAGE_MAKE:
        return STRING_MAKE;
    default:
        return "";
    }
}
