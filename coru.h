#ifndef CORU_RUN_H
#define CORU_RUN_H

#include <stdio.h>

/* Custom boolean type */
#if __STDC_VERSION__ < 199901L
    #ifndef __cplusplus
        #ifndef BOOL
            typedef char BOOL;
            #define FALSE  0
            #define TRUE   1
        #endif
    #else
        typedef bool BOOL;
        #define FALSE  false
        #define TRUE   true
    #endif
#else
    #ifndef __cplusplus
        #include <stdbool.h>
    #else
        #include <cstdbool>
    #endif

    typedef bool   BOOL;
    #define FALSE  false
    #define TRUE   true
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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct coru_argument_t coru_argument_t;

coru_argument_t * coru_argument_parse(int argc, char **argv);
void coru_argument_delete(void *self);


typedef struct coru_stats_t coru_stats_t;

coru_stats_t * coru_stats_load(FILE *stream);
void coru_stats_delete(void *self);


BOOL coru_run(coru_argument_t *arg, char **out);
BOOL coru_load_all(FILE *stream, char **out, coru_stats_t *stats, language_t lang);
BOOL coru_load_non_empty(FILE *stream, char **out, coru_stats_t *stats, language_t lang);

#ifdef __cplusplus
}
#endif

#endif  /* CORU_H */
