#ifndef LAI_UTILS_H
#define LAI_UTILS_H

#include <stdio.h>
#include "boolean.h"
#include "command.h"
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

BOOL is_command_equal(COMMAND a, COMMAND b);
char * detect_target_language(char *path);
BOOL is_string_equal(char *a, char *b);

#ifndef __APPLE__
#if __STDC_VERSION__ < 199901L
    int snprintf( char * s, size_t n, const char * format, ... );
#endif
#endif

#endif  /* LAI_UTILS_H */
