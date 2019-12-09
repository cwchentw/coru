#ifndef CORU_RUN_H
#define CORU_RUN_H

#ifdef __cplusplus
    #include <cstdio>
#else
    #include <stdio.h>
#endif

/* Custom boolean type */
#ifdef __cplusplus
    typedef bool BOOL;
    #define FALSE  false
    #define TRUE   true
#else
    #if __STDC_VERSION__ < 199901L
        #ifndef BOOL
            typedef char BOOL;
            #define FALSE  0
            #define TRUE   1
        #endif
    #else
        #include <stdbool.h>
        typedef bool BOOL;
        #define FALSE  false
        #define TRUE   true
    #endif
#endif

/* Valid target language. */
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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct coru_stats_t coru_stats_t;

coru_stats_t * coru_stats_load(FILE *stream);
void coru_stats_delete(void *self);

BOOL coru_run(int argc, char **argv, char **out);
BOOL coru_load_all(FILE *stream, coru_stats_t *stats, language_t lang, char **out);
BOOL coru_load_non_empty(FILE *stream, coru_stats_t *stats, language_t lang, char **out);

#ifdef __cplusplus
}
#endif

#endif  /* CORU_H */
