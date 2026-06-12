#ifndef LANGUAGE_H
#define LANGUAGE_H

#ifdef _WIN32
    #include <windows.h>
#endif

#include "boolean.h"
#include "coru.h"

#define STRING_C       "C"
#define STRING_CPP     "C++"
#define STRING_PASCAL  "Pascal"
#define STRING_FORTRAN "Fortran"
#define STRING_CL      "Common Lisp"
#define STRING_JAVA    "Java"
#define STRING_CSHARP  "C sharp"
#define STRING_PERL    "Perl"
#define STRING_PYTHON  "Python"
#define STRING_RUBY    "Ruby"
#define STRING_PHP     "PHP"
#define STRING_JS      "JavaScript"
#define STRING_GO      "Golang"
#define STRING_RUST    "Rust"
#define STRING_OBJC    "Objective-C"
#define STRING_OBJCPP  "Objective-C++"
#define STRING_SWIFT   "Swift"
#define STRING_CSH     "C Shell"
#define STRING_SH      "Bourne Shell"
#define STRING_PS      "PowerShell"
#define STRING_CMAKE   "CMake"
#define STRING_MAKE    "Make"

language_t detect_target_language(char *path);
const char * language_to_string(language_t lang);

#define is_language_equal(a, b) ((a) == (b))

#endif  /* CORU_LANGUAGE_H */
