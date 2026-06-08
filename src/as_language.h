#ifndef _AS_LANGUAGE_H
#define _AS_LANGUAGE_H

#include "language.h"

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

#define as_language(arg, opt) \
    else if (_is_c((opt))) { \
        (arg)->lang = LANGUAGE_C; \
        (arg)->index += 1; \
    } \
    else if (_is_cpp((opt))) { \
        (arg)->lang = LANGUAGE_CPP; \
        (arg)->index += 1; \
    } \
    else if (_is_pascal((opt))) { \
        (arg)->lang = LANGUAGE_PASCAL; \
        (arg)->index += 1; \
    } \
    else if (_is_java((opt))) { \
        (arg)->lang = LANGUAGE_JAVA; \
        (arg)->index += 1; \
    } \
    else if (_is_csharp((opt))) { \
        (arg)->lang = LANGUAGE_CSHARP; \
        (arg)->index += 1; \
    } \
    else if (_is_perl((opt))) { \
        (arg)->lang = LANGUAGE_PERL; \
        (arg)->index += 1; \
    } \
    else if (_is_python((opt))) { \
        (arg)->lang = LANGUAGE_PYTHON; \
        (arg)->index += 1; \
    } \
    else if (_is_ruby((opt))) { \
        (arg)->lang = LANGUAGE_RUBY; \
        (arg)->index += 1; \
    } \
    else if (_is_php((opt))) { \
        (arg)->lang = LANGUAGE_PHP; \
        (arg)->index += 1; \
    } \
    else if (_is_js((opt))) { \
        (arg)->lang = LANGUAGE_JS; \
        (arg)->index += 1; \
    } \
    else if (_is_golang((opt))) { \
        (arg)->lang = LANGUAGE_GO; \
        (arg)->index += 1; \
    } \
    else if (_is_rust((opt))) { \
        (arg)->lang = LANGUAGE_RUST; \
        (arg)->index += 1; \
    } \
    else if (_is_objc((opt))) { \
        (arg)->lang = LANGUAGE_OBJC; \
        (arg)->index += 1; \
    } \
    else if (_is_swift((opt))) { \
        (arg)->lang = LANGUAGE_SWIFT; \
        (arg)->index += 1; \
    } \
    else if (_is_csh((opt))) { \
        (arg)->lang = LANGUAGE_CSH; \
        (arg)->index += 1; \
    } \
    else if (_is_sh((opt))) { \
        (arg)->lang = LANGUAGE_SH; \
        (arg)->index += 1; \
    } \
    else if (_is_cmake((opt))) { \
        (arg)->lang = LANGUAGE_CMAKE; \
        (arg)->index += 1; \
    } \
    else if (_is_make((opt))) { \
        (arg)->lang = LANGUAGE_MAKE; \
        (arg)->index += 1; \
    }

#endif  /* _AS_LANGUAGE_H */