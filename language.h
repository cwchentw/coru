#ifndef CORU_LANGUAGE_H
#define CORU_LANGUAGE_H

#include "boolean.h"

typedef char language_t;

#define LANGUAGE_UNKNOWN   0
#define LANGUAGE_C         1
#define LANGUAGE_CPP       2
#define LANGUAGE_JAVA      3
#define LANGUAGE_CSHARP    4
#define LANGUAGE_PERL      5
#define LANGUAGE_PYTHON    6
#define LANGUAGE_RUBY      7
#define LANGUAGE_PHP       8
#define LANGUAGE_JS        9
#define LANGUAGE_GO       10
#define LANGUAGE_RUST     11
#define LANGUAGE_OBJC     12
#define LANGUAGE_OBJCPP   13
#define LANGUAGE_SWIFT    14
#define LANGUAGE_CSH      15
#define LANGUAGE_SH       16

#define STRING_C       "C"
#define STRING_CPP     "C++"
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

language_t detect_target_language(char *path);
BOOL is_language_equal(language_t a, language_t b);
char * language_to_string(language_t lang);

#endif  /* CORU_LANGUAGE_H */
