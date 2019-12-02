#ifndef LAI_LANGUAGE_H
#define LAI_LANGUAGE_H

#include "boolean.h"

typedef char language_t;

#define LANGUAGE_UNKNOWN  0
#define LANGUAGE_C        1
#define LANGUAGE_CPP      2
#define LANGUAGE_OBJC     3
#define LANGUAGE_OBJCPP   4
#define LANGUAGE_SH       5

language_t detect_target_language(char *path);
BOOL is_language_equal(language_t a, language_t b);
char * language_to_string(language_t lang);

#endif  /* LAI_LANGUAGE_H */
