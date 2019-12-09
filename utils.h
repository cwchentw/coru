#ifndef CORU_UTILS_H
#define CORU_UTILS_H

#include <stdio.h>
#include "boolean.h"
#include "platform.h"


#define PRINT(format, ...) { \
        fprintf(stdout, format, ##__VA_ARGS__); \
    }

#define PERROR(format, ...) { \
        fprintf(stderr, format, ##__VA_ARGS__); \
    }

#define PUTS(format, ...) { \
        fprintf(stdout, format "%s", ##__VA_ARGS__, END_OF_LINE); \
    }

#define PUTERR(format, ...) { \
        fprintf(stderr, format "%s", ##__VA_ARGS__, END_OF_LINE); \
    }


BOOL is_string_equal(char *a, char *b);
BOOL string_starts_with(char *a, char *b);
BOOL string_contains(char *a, char *b);

#endif  /* CORU_UTILS_H */
