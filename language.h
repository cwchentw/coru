#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "boolean.h"
#include "coru.h"

language_t detect_target_language(char *path);
BOOL is_language_equal(language_t a, language_t b);
char * language_to_string(language_t lang);

#endif  /* CORU_LANGUAGE_H */
